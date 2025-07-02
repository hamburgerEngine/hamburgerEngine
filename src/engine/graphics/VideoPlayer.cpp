#include "VideoPlayer.h"
#include "../core/SDLManager.h"
#include "../utils/Log.h"
#include "../core/Engine.h"

void* VideoPlayer::lock(void* data, void** p_pixels) {
    VideoPlayer* player = static_cast<VideoPlayer*>(data);
    SDL_LockMutex(player->mutex);
    *p_pixels = player->videoPixels;
    return nullptr;
}

void VideoPlayer::unlock(void* data, void* id, void* const* p_pixels) {
    VideoPlayer* player = static_cast<VideoPlayer*>(data);
    SDL_UnlockMutex(player->mutex);
    (void)id;
    (void)p_pixels;
}

void VideoPlayer::display(void* data, void* id) {
    (void)data;
    (void)id;
}

VideoPlayer::VideoPlayer() 
    : vlcInstance(nullptr), mediaPlayer(nullptr), media(nullptr), playing(false),
      videoTexture(nullptr), mutex(nullptr), videoWidth(0), videoHeight(0), videoPixels(nullptr) {
    vlcInstance = libvlc_new(0, nullptr);
    if (!vlcInstance) {
        Log::getInstance().error("Failed to initialize VLC instance");
        return;
    }
    mutex = SDL_CreateMutex();
}

VideoPlayer::~VideoPlayer() {
    stop();
    if (mediaPlayer) {
        libvlc_media_player_release(mediaPlayer);
        mediaPlayer = nullptr;
    }
    if (media) {
        libvlc_media_release(media);
        media = nullptr;
    }
    if (vlcInstance) {
        libvlc_release(vlcInstance);
        vlcInstance = nullptr;
    }
    if (videoTexture) {
        SDL_DestroyTexture(videoTexture);
        videoTexture = nullptr;
    }
    if (mutex) {
        SDL_DestroyMutex(mutex);
        mutex = nullptr;
    }
    if (videoPixels) {
        free(videoPixels);
        videoPixels = nullptr;
    }
}

bool VideoPlayer::loadVideo(const std::string& path) {
    Log::getInstance().info("Attempting to load video: " + path);
    
    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        Log::getInstance().error("Video file not found: " + path);
        return false;
    }
    fclose(file);
    
    if (!vlcInstance) {
        Log::getInstance().error("VLC instance not initialized");
        return false;
    }

    if (mediaPlayer) {
        libvlc_media_player_release(mediaPlayer);
        mediaPlayer = nullptr;
    }
    if (media) {
        libvlc_media_release(media);
        media = nullptr;
    }

    media = libvlc_media_new_path(vlcInstance, path.c_str());
    if (!media) {
        media = libvlc_media_new_location(vlcInstance, path.c_str());
        if (!media) {
            Log::getInstance().error("Failed to create media from path: " + path);
            if (const char* err = libvlc_errmsg()) {
                Log::getInstance().error("VLC Error: " + std::string(err));
            }
            return false;
        }
    }
    libvlc_media_add_option(media, ":network-caching=1000");
    libvlc_media_add_option(media, ":file-caching=1000");    
    libvlc_media_parse_with_options(media, libvlc_media_parse_local, -1);
    
    libvlc_media_parsed_status_t status;
    do {
        status = libvlc_media_get_parsed_status(media);
        if (status == libvlc_media_parsed_status_failed) {
            Log::getInstance().error("Media parsing failed");
            if (const char* err = libvlc_errmsg()) {
                Log::getInstance().error("VLC Error: " + std::string(err));
            }
            libvlc_media_release(media);
            media = nullptr;
            return false;
        }
    } while (status != libvlc_media_parsed_status_done);
    
    mediaPlayer = libvlc_media_player_new_from_media(media);
    if (!mediaPlayer) {
        Log::getInstance().error("Failed to create media player");
        if (const char* err = libvlc_errmsg()) {
            Log::getInstance().error("VLC Error: " + std::string(err));
        }
        libvlc_media_release(media);
        media = nullptr;
        return false;
    }

    libvlc_media_track_t** tracks;
    unsigned tracksCount = libvlc_media_tracks_get(media, &tracks);
    
    bool foundVideo = false;
    for (unsigned i = 0; i < tracksCount; i++) {
        const char* type;
        switch (tracks[i]->i_type) {
            case libvlc_track_audio: type = "audio"; break;
            case libvlc_track_video: type = "video"; break;
            case libvlc_track_text: type = "text"; break;
            default: type = "unknown"; break;
        }
        
        if (tracks[i]->i_type == libvlc_track_video) {
            videoWidth = tracks[i]->video->i_width;
            videoHeight = tracks[i]->video->i_height;
            foundVideo = true;
            break;
        }
    }

    if (tracksCount > 0) {
        libvlc_media_tracks_release(tracks, tracksCount);
    }
    
    libvlc_media_release(media);
    media = nullptr;

    if (!foundVideo) {
        Log::getInstance().error("No video track found in media. File may be corrupted or in an unsupported format");
        if (const char* err = libvlc_errmsg()) {
            Log::getInstance().error("VLC Error: " + std::string(err));
        }
        return false;
    }

    if (videoPixels) {
        free(videoPixels);
    }
    if (videoTexture) {
        SDL_DestroyTexture(videoTexture);
    }

    videoPixels = malloc(videoWidth * videoHeight * 4);
    if (!videoPixels) {
        Log::getInstance().error("Failed to allocate video buffer of size: " + 
            std::to_string(videoWidth * videoHeight * 4) + " bytes");
        return false;
    }

    SDL_Renderer* renderer = SDLManager::getInstance().getRenderer();
    videoTexture = SDL_CreateTexture(renderer, 
                                   SDL_PIXELFORMAT_RGB888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   videoWidth, videoHeight);
    if (!videoTexture) {
        Log::getInstance().error("Failed to create video texture. SDL Error: " + std::string(SDL_GetError()));
        return false;
    }

    libvlc_video_set_callbacks(mediaPlayer, lock, unlock, display, this);
    libvlc_video_set_format(mediaPlayer, "RV32", videoWidth, videoHeight, videoWidth * 4);
    return true;
}

void VideoPlayer::play() {
    if (mediaPlayer) {
        libvlc_media_player_play(mediaPlayer);
        playing = true;
    } else {
        Log::getInstance().error("Cannot play: no media loaded");
    }
}

void VideoPlayer::pause() {
    if (mediaPlayer) {
        libvlc_media_player_pause(mediaPlayer);
        playing = false;
    } else {
        Log::getInstance().error("Cannot pause: no media loaded");
    }
}

void VideoPlayer::stop() {
    if (mediaPlayer) {
        libvlc_media_player_stop(mediaPlayer);
        playing = false;
    } else {
        Log::getInstance().error("Cannot stop: no media loaded");
    }
}

void VideoPlayer::setVolume(int volume) {
    if (mediaPlayer) {
        volume = std::max(0, std::min(100, volume));
        libvlc_audio_set_volume(mediaPlayer, volume);
    } else {
        Log::getInstance().error("Cannot set volume: no media loaded");
    }
}

void VideoPlayer::setPosition(float pos) {
    if (mediaPlayer) {
        pos = std::max(0.0f, std::min(1.0f, pos));
        libvlc_media_player_set_position(mediaPlayer, pos);
    } else {
        Log::getInstance().error("Cannot set position: no media loaded");
    }
}

float VideoPlayer::getPosition() const {
    if (mediaPlayer) {
        return libvlc_media_player_get_position(mediaPlayer);
    }
    return 0.0f;
}

bool VideoPlayer::isPlaying() const {
    if (mediaPlayer) {
        return libvlc_media_player_is_playing(mediaPlayer) != 0;
    }
    return false;
}

void VideoPlayer::update() {}

void VideoPlayer::render(SDL_Renderer* renderer) {
    if (!videoTexture || !playing) return;

    SDL_LockMutex(mutex);
    SDL_UpdateTexture(videoTexture, nullptr, videoPixels, videoWidth * 4);
    SDL_UnlockMutex(mutex);

    SDL_Rect dstRect;
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    
    float videoAspect = (float)videoWidth / videoHeight;
    float windowAspect = (float)windowWidth / windowHeight;
    
    if (windowAspect > videoAspect) {
        dstRect.h = windowHeight;
        dstRect.w = (int)(windowHeight * videoAspect);
        dstRect.x = (windowWidth - dstRect.w) / 2;
        dstRect.y = 0;
    } else {
        dstRect.w = windowWidth;
        dstRect.h = (int)(windowWidth / videoAspect);
        dstRect.x = 0;
        dstRect.y = (windowHeight - dstRect.h) / 2;
    }

    SDL_RenderCopy(renderer, videoTexture, nullptr, &dstRect);
}