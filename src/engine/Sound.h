#pragma once
#include <string>
#include <SDL2/SDL_mixer.h>

class Sound {
public:
    Sound();
    ~Sound();

    bool load(const std::string& path);
    void play();
    void pause();
    void resume();
    void stop();
    void setVolume(float volume);
    void setLoop(bool loop);
    bool isPlaying() const;

private:
    Mix_Chunk* sound;
    bool isLoaded;
    bool playing;
    bool looping;
    float volume;
    int channel;
}; 