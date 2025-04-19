#pragma once
#include <map>
#include <string>
#include "Sound.h"
#include <SDL2/SDL_mixer.h>

class SoundManager {
public:
    static SoundManager& getInstance();
    
    void playMusic(const std::string& path, float volume = 1.0f);
    void loopMusic(const std::string& path, float volume = 1.0f, int loops = -1);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    void setMusicVolume(float volume);

    Sound* loadSound(const std::string& path);
    void playSound(const std::string& path, float volume = 1.0f);

private:
    SoundManager();
    ~SoundManager();
    
    std::map<std::string, Sound*> sounds;
    Mix_Music* currentMusic;
}; 