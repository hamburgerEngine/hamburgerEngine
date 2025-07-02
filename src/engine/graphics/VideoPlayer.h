#pragma once

#include <string>
#include <queue>
#include <vlc/vlc.h>
#include <SDL2/SDL.h>

class VideoPlayer {
public:
    VideoPlayer();
    ~VideoPlayer();

    bool loadVideo(const std::string& path);
    void play();
    void pause();
    void stop();
    void setVolume(int volume); // 0-100
    void setPosition(float pos); // 0.0-1.0
    float getPosition() const;
    bool isPlaying() const;
    void update();
    void render(SDL_Renderer* renderer);

private:
    static void* lock(void* data, void** p_pixels);
    static void unlock(void* data, void* id, void* const* p_pixels);
    static void display(void* data, void* id);

    libvlc_instance_t* vlcInstance;
    libvlc_media_player_t* mediaPlayer;
    libvlc_media_t* media;
    bool playing;

    SDL_Texture* videoTexture;
    SDL_mutex* mutex;
    unsigned videoWidth;
    unsigned videoHeight;
    void* videoPixels;
}; 
