#include "VideoPlayer.h"
#include "SDLManager.h"
#include "Log.h"
#include "Engine.h"

VideoPlayer::VideoPlayer()
    : formatContext(nullptr), videoCodecContext(nullptr), audioCodecContext(nullptr),
      frame(nullptr), frameRGB(nullptr), swsContext(nullptr), swrContext(nullptr),
      texture(nullptr), videoStream(-1), audioStream(-1), videoWidth(0), videoHeight(0),
      windowWidth(0), windowHeight(0), duration(0), currentTime(0), frameTime(0),
      timeSinceLastFrame(0), fps(0), volume(1.0f), playing(false), maintainAspectRatio(true),
      hasAudio(false), buffer(nullptr), audioChunk(nullptr), audioClock(0), audioCallbackTime(0) {
    renderRect = {0, 0, 0, 0};
    Engine* engine = Engine::getInstance();
    if (engine) {
        setWindowSize(engine->getWindowWidth(), engine->getWindowHeight());
    }
}

VideoPlayer::~VideoPlayer() {
    cleanup();
}

void VideoPlayer::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    Log::getInstance().info("Video window size set to: " + std::to_string(width) + "x" + std::to_string(height));
    calculateRenderRect();
}

void VideoPlayer::calculateRenderRect() {
    if (videoWidth == 0 || videoHeight == 0 || windowWidth == 0 || windowHeight == 0) {
        return;
    }

    renderRect = {0, 0, windowWidth, windowHeight};
    Log::getInstance().info("Video render rect set to: " + 
                          std::to_string(renderRect.x) + "," + 
                          std::to_string(renderRect.y) + "," + 
                          std::to_string(renderRect.w) + "," + 
                          std::to_string(renderRect.h));
}

bool VideoPlayer::load(const std::string& filePath) {
    cleanup();

    if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) < 0) {
        Log::getInstance().error("Could not open video file: " + filePath);
        return false;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        Log::getInstance().error("Could not find stream info");
        cleanup();
        return false;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1) {
        Log::getInstance().error("Could not find video stream");
        cleanup();
        return false;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStream = i;
            break;
        }
    }

    AVRational timeBase = formatContext->streams[videoStream]->time_base;
    AVRational frameRate = formatContext->streams[videoStream]->r_frame_rate;
    fps = frameRate.num / (float)frameRate.den;
    frameTime = 1.0f / fps;

    Log::getInstance().info("Video FPS: " + std::to_string(fps));
    Log::getInstance().info("Frame time: " + std::to_string(frameTime) + " seconds");

    const AVCodec* videoCodec = avcodec_find_decoder(formatContext->streams[videoStream]->codecpar->codec_id);
    if (!videoCodec) {
        Log::getInstance().error("Unsupported video codec");
        cleanup();
        return false;
    }

    videoCodecContext = avcodec_alloc_context3(videoCodec);
    if (!videoCodecContext) {
        Log::getInstance().error("Could not allocate video codec context");
        cleanup();
        return false;
    }

    if (avcodec_parameters_to_context(videoCodecContext, formatContext->streams[videoStream]->codecpar) < 0) {
        Log::getInstance().error("Could not fill video codec context");
        cleanup();
        return false;
    }

    if (avcodec_open2(videoCodecContext, videoCodec, nullptr) < 0) {
        Log::getInstance().error("Could not open video codec");
        cleanup();
        return false;
    }

    if (audioStream != -1) {
        hasAudio = initializeAudio();
        if (hasAudio) {
            Log::getInstance().info("Audio stream initialized successfully");
        } else {
            Log::getInstance().warning("Failed to initialize audio stream");
        }
    }

    videoWidth = videoCodecContext->width;
    videoHeight = videoCodecContext->height;
    duration = static_cast<float>(formatContext->duration) / AV_TIME_BASE;

    frame = av_frame_alloc();
    frameRGB = av_frame_alloc();
    if (!frame || !frameRGB) {
        cleanup();
        return false;
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, videoWidth, videoHeight, 1);
    buffer = (uint8_t*)av_malloc(numBytes);
    av_image_fill_arrays(frameRGB->data, frameRGB->linesize, buffer,
                        AV_PIX_FMT_RGB24, videoWidth, videoHeight, 1);

    swsContext = sws_getContext(videoWidth, videoHeight, videoCodecContext->pix_fmt,
                               videoWidth, videoHeight, AV_PIX_FMT_RGB24,
                               SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!swsContext) {
        cleanup();
        return false;
    }

    texture = SDL_CreateTexture(SDLManager::getInstance().getRenderer(),
                              SDL_PIXELFORMAT_RGB24,
                              SDL_TEXTUREACCESS_STREAMING,
                              videoWidth, videoHeight);

    if (!texture) {
        cleanup();
        return false;
    }

    Engine* engine = Engine::getInstance();
    if (engine) {
        setWindowSize(engine->getWindowWidth(), engine->getWindowHeight());
    }

    return true;
}

bool VideoPlayer::initializeAudio() {
    const AVCodec* audioCodec = avcodec_find_decoder(formatContext->streams[audioStream]->codecpar->codec_id);
    if (!audioCodec) {
        Log::getInstance().error("Unsupported audio codec");
        return false;
    }

    audioCodecContext = avcodec_alloc_context3(audioCodec);
    if (!audioCodecContext) {
        Log::getInstance().error("Could not allocate audio codec context");
        return false;
    }

    if (avcodec_parameters_to_context(audioCodecContext, formatContext->streams[audioStream]->codecpar) < 0) {
        Log::getInstance().error("Could not fill audio codec context");
        return false;
    }

    if (avcodec_open2(audioCodecContext, audioCodec, nullptr) < 0) {
        Log::getInstance().error("Could not open audio codec");
        return false;
    }

    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, AUDIO_CHUNK_SIZE) < 0) {
        Log::getInstance().error("SDL_mixer initialization failed: " + std::string(Mix_GetError()));
        return false;
    }

    Mix_AllocateChannels(4);

    swrContext = swr_alloc();
    if (!swrContext) {
        Log::getInstance().error("Could not allocate resampler context");
        return false;
    }

    AVChannelLayout out_ch_layout = AV_CHANNEL_LAYOUT_STEREO;
    AVChannelLayout in_ch_layout = audioCodecContext->ch_layout;
    if (in_ch_layout.nb_channels == 0) {
        av_channel_layout_default(&in_ch_layout, 2);
    }

    int ret = swr_alloc_set_opts2(&swrContext,
        &out_ch_layout,
        AV_SAMPLE_FMT_S16,
        44100,
        &in_ch_layout,
        audioCodecContext->sample_fmt,
        audioCodecContext->sample_rate,
        0,
        nullptr);

    if (ret < 0) {
        Log::getInstance().error("Could not set resampler options");
        return false;
    }

    if (swr_init(swrContext) < 0) {
        Log::getInstance().error("Could not initialize resampler");
        return false;
    }

    Log::getInstance().info("Audio initialized with sample rate: " + std::to_string(audioCodecContext->sample_rate));
    return true;
}

void VideoPlayer::processAudioFrame(AVFrame* audioFrame) {
    if (!hasAudio || !audioFrame) return;

    if (audioQueue.size() >= MAX_AUDIO_QUEUE_SIZE) {
        return;
    }

    double pts = audioFrame->pts;
    if (pts != AV_NOPTS_VALUE) {
        pts = av_q2d(formatContext->streams[audioStream]->time_base) * pts;
    }

    int out_samples = av_rescale_rnd(
        swr_get_delay(swrContext, audioCodecContext->sample_rate) + audioFrame->nb_samples,
        44100,
        audioCodecContext->sample_rate,
        AV_ROUND_UP);

    uint8_t* output_buffer = (uint8_t*)malloc(AUDIO_BUFFER_SIZE);
    if (!output_buffer) {
        Log::getInstance().error("Failed to allocate audio output buffer");
        return;
    }

    int samples_converted = swr_convert(swrContext,
        &output_buffer, out_samples,
        (const uint8_t**)audioFrame->data, audioFrame->nb_samples);

    if (samples_converted > 0) {
        int actual_buffer_size = av_samples_get_buffer_size(nullptr, 2, samples_converted, AV_SAMPLE_FMT_S16, 0);
        if (actual_buffer_size > 0) {
            queueAudio(output_buffer, actual_buffer_size, pts);
        } else {
            free(output_buffer);
        }
    } else {
        free(output_buffer);
    }
}

void VideoPlayer::queueAudio(uint8_t* data, int size, double pts) {
    if (!data || size <= 0) return;

    AudioBuffer buffer(data, size, pts);
    audioQueue.push(buffer);

    if (Mix_Playing(-1) == 0 && !audioQueue.empty()) {
        AudioBuffer& front = audioQueue.front();
        Mix_Chunk* chunk = Mix_QuickLoad_RAW(front.data, front.size);
        if (chunk) {
            Mix_VolumeChunk(chunk, static_cast<int>(volume * MIX_MAX_VOLUME));
            if (Mix_PlayChannel(-1, chunk, 0) != -1) {
                audioClock = front.pts;
                audioCallbackTime = SDL_GetTicks() / 1000.0;
                audioChunk = chunk;
            } else {
                Mix_FreeChunk(chunk);
            }
        }
        free(front.data);
        audioQueue.pop();
    }
}

double VideoPlayer::getAudioClock() {
    double delta = (SDL_GetTicks() / 1000.0) - audioCallbackTime;
    return audioClock + delta;
}

void VideoPlayer::update(float deltaTime) {
    if (!playing) return;

    double videoTime = currentTime;
    double audioTime = hasAudio ? getAudioClock() : videoTime;
    
    if (hasAudio) {
        double diff = videoTime - audioTime;
        
        if (fabs(diff) > AUDIO_SYNC_THRESHOLD) {
            if (diff > 0) {
                deltaTime *= 0.9f;
            } else {
                deltaTime *= 1.1f;
            }
        }
    }

    currentTime += deltaTime;
    timeSinceLastFrame += deltaTime;

    if (hasAudio && Mix_Playing(-1) == 0 && !audioQueue.empty()) {
        if (audioChunk) {
            Mix_FreeChunk(audioChunk);
            audioChunk = nullptr;
        }

        AudioBuffer& front = audioQueue.front();
        Mix_Chunk* chunk = Mix_QuickLoad_RAW(front.data, front.size);
        if (chunk) {
            Mix_VolumeChunk(chunk, static_cast<int>(volume * MIX_MAX_VOLUME));
            if (Mix_PlayChannel(-1, chunk, 0) != -1) {
                audioClock = front.pts;
                audioCallbackTime = SDL_GetTicks() / 1000.0;
                audioChunk = chunk;
            } else {
                Mix_FreeChunk(chunk);
            }
        }
        free(front.data);
        audioQueue.pop();
    }

    if (timeSinceLastFrame >= frameTime) {
        if (decodeFrame()) {
            convertFrame();
            timeSinceLastFrame = fmod(timeSinceLastFrame, frameTime);
        }
    }
}

void VideoPlayer::render(SDL_Renderer* renderer) {
    if (!texture || !renderer) return;
    SDL_RenderCopy(renderer, texture, nullptr, &renderRect);
}

void VideoPlayer::play() {
    playing = true;
    Mix_Resume(-1);
}

void VideoPlayer::pause() {
    playing = false;
    Mix_Pause(-1);
}

void VideoPlayer::stop() {
    playing = false;
    currentTime = 0;
    Mix_HaltChannel(-1);
    cleanupAudioQueue();
    av_seek_frame(formatContext, videoStream, 0, AVSEEK_FLAG_BACKWARD);
}

void VideoPlayer::seek(float time) {
    if (!formatContext) return;

    Mix_HaltChannel(-1);
    cleanupAudioQueue();
    
    int64_t timestamp = static_cast<int64_t>(time * AV_TIME_BASE);
    av_seek_frame(formatContext, -1, timestamp, AVSEEK_FLAG_BACKWARD);
    currentTime = time;
    
    if (hasAudio && audioCodecContext) {
        avcodec_flush_buffers(audioCodecContext);
    }
}

bool VideoPlayer::decodeFrame() {
    AVPacket* packet = av_packet_alloc();
    bool frameDecoded = false;
    int ret;

    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStream) {
            ret = avcodec_send_packet(videoCodecContext, packet);
            if (ret < 0) {
                av_packet_free(&packet);
                return false;
            }

            ret = avcodec_receive_frame(videoCodecContext, frame);
            if (ret >= 0) {
                frameDecoded = true;
                break;
            }
        } else if (packet->stream_index == audioStream && hasAudio) {
            ret = avcodec_send_packet(audioCodecContext, packet);
            if (ret >= 0) {
                AVFrame* audioFrame = av_frame_alloc();
                ret = avcodec_receive_frame(audioCodecContext, audioFrame);
                if (ret >= 0) {
                    processAudioFrame(audioFrame);
                }
                av_frame_free(&audioFrame);
            }
        }
        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    return frameDecoded;
}

void VideoPlayer::convertFrame() {
    if (!frame || !frameRGB || !swsContext || !texture) return;

    sws_scale(swsContext,
             frame->data, frame->linesize, 0, videoHeight,
             frameRGB->data, frameRGB->linesize);

    SDL_UpdateTexture(texture, nullptr, frameRGB->data[0], frameRGB->linesize[0]);
}

void VideoPlayer::setVolume(float vol) {
    volume = vol;
    if (audioChunk) {
        Mix_VolumeChunk(audioChunk, static_cast<int>(volume * MIX_MAX_VOLUME));
    }
}

void VideoPlayer::cleanupAudioQueue() {
    if (audioChunk) {
        Mix_FreeChunk(audioChunk);
        audioChunk = nullptr;
    }

    while (!audioQueue.empty()) {
        AudioBuffer& buffer = audioQueue.front();
        if (buffer.data) {
            free(buffer.data);
        }
        audioQueue.pop();
    }
}

void VideoPlayer::cleanup() {
    if (swrContext) {
        swr_free(&swrContext);
        swrContext = nullptr;
    }

    if (audioCodecContext) {
        avcodec_free_context(&audioCodecContext);
        audioCodecContext = nullptr;
    }

    if (swsContext) {
        sws_freeContext(swsContext);
        swsContext = nullptr;
    }

    if (frameRGB) {
        av_frame_free(&frameRGB);
        frameRGB = nullptr;
    }

    if (frame) {
        av_frame_free(&frame);
        frame = nullptr;
    }

    if (videoCodecContext) {
        avcodec_free_context(&videoCodecContext);
        videoCodecContext = nullptr;
    }

    if (formatContext) {
        avformat_close_input(&formatContext);
        formatContext = nullptr;
    }

    if (buffer) {
        av_free(buffer);
        buffer = nullptr;
    }

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    Mix_HaltChannel(-1);
    cleanupAudioQueue();
    
    if (audioChunk) {
        Mix_FreeChunk(audioChunk);
        audioChunk = nullptr;
    }

    videoStream = -1;
    audioStream = -1;
    videoWidth = 0;
    videoHeight = 0;
    duration = 0;
    currentTime = 0;
    playing = false;
    hasAudio = false;
} 