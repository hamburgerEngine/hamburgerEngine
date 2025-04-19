#pragma once

#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

class VideoPlayer {
public:
    VideoPlayer();
    ~VideoPlayer();

    bool load(const std::string& filePath);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void play();
    void pause();
    void stop();
    void seek(float time);
    
    bool isPlaying() const { return playing; }
    float getDuration() const { return duration; }
    float getCurrentTime() const { return currentTime; }
    int getWidth() const { return videoWidth; }
    int getHeight() const { return videoHeight; }
    float getFPS() const { return fps; }

    void setWindowSize(int width, int height);
    void setMaintainAspectRatio(bool maintain) { maintainAspectRatio = maintain; }
    bool getMaintainAspectRatio() const { return maintainAspectRatio; }
    void setVolume(float vol);

private:
    struct AudioBuffer {
        uint8_t* data;
        int size;
        double pts;
        AudioBuffer() : data(nullptr), size(0), pts(0) {}
        AudioBuffer(uint8_t* d, int s, double p) : data(d), size(s), pts(p) {}
    };

    static constexpr int MAX_AUDIO_QUEUE_SIZE = 8;
    static constexpr int AUDIO_BUFFER_SIZE = 1024 * 8;
    static constexpr int AUDIO_CHUNK_SIZE = 8192;
    static constexpr float AUDIO_SYNC_THRESHOLD = 0.1f;

    void cleanup();
    bool decodeFrame();
    void convertFrame();
    void calculateRenderRect();
    bool initializeAudio();
    void processAudioFrame(AVFrame* frame);
    void cleanupAudioQueue();
    double getAudioClock();
    void queueAudio(uint8_t* data, int size, double pts);

    AVFormatContext* formatContext;
    AVCodecContext* videoCodecContext;
    AVCodecContext* audioCodecContext;
    AVFrame* frame;
    AVFrame* frameRGB;
    SwsContext* swsContext;
    SwrContext* swrContext;
    SDL_Texture* texture;
    
    int videoStream;
    int audioStream;
    int videoWidth;
    int videoHeight;
    int windowWidth;
    int windowHeight;
    float duration;
    float currentTime;
    float frameTime;
    float timeSinceLastFrame;
    float fps;
    float volume;
    bool playing;
    bool maintainAspectRatio;
    bool hasAudio;
    uint8_t* buffer;
    SDL_Rect renderRect;
    Mix_Chunk* audioChunk;
    std::queue<AudioBuffer> audioQueue;
    double audioClock;
    double audioCallbackTime;
}; 