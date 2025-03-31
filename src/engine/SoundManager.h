#pragma once
#include <map>
#include <string>
#include "Sound.h"

class SoundManager {
public:
    static SoundManager& getInstance();
    
    void playMusic(const std::string& path, float volume = 1.0f);
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
    Sound* currentMusic;
}; 