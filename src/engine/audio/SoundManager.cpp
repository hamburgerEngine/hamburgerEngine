#include "SoundManager.h"
#include <iostream>
#include "Log.h"

SoundManager::SoundManager() : currentMusic(nullptr) {}

SoundManager::~SoundManager() {
    for (auto& pair : sounds) {
        delete pair.second;
    }
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
    }
}

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::playMusic(const std::string& path, float volume) {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
    }

    currentMusic = Mix_LoadMUS(path.c_str());
    if (!currentMusic) {
        Log::getInstance().error("Failed to load music: " + std::string(Mix_GetError()));
        return;
    }

    Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
    if (Mix_PlayMusic(currentMusic, -1) == -1) {
        Log::getInstance().error("Failed to play music: " + std::string(Mix_GetError()));
    }
}

void SoundManager::loopMusic(const std::string& path, float volume, int loops) {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
    }

    currentMusic = Mix_LoadMUS(path.c_str());
    if (!currentMusic) {
        Log::getInstance().error("Failed to load music: " + std::string(Mix_GetError()));
        return;
    }

    Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
    if (Mix_PlayMusic(currentMusic, loops) == -1) {
        Log::getInstance().error("Failed to play music: " + std::string(Mix_GetError()));
    }
}

void SoundManager::pauseMusic() {
    if (currentMusic && Mix_PlayingMusic()) {
        Mix_PauseMusic();
    }
}

void SoundManager::resumeMusic() {
    if (currentMusic && Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void SoundManager::stopMusic() {
    if (currentMusic) {
        Mix_HaltMusic();
    }
}

void SoundManager::setMusicVolume(float volume) {
    Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
}

Sound* SoundManager::loadSound(const std::string& path) {
    auto it = sounds.find(path);
    if (it != sounds.end()) {
        return it->second;
    }

    Sound* sound = new Sound();
    if (sound->load(path)) {
        sounds[path] = sound;
        return sound;
    }
    
    delete sound;
    return nullptr;
}

void SoundManager::playSound(const std::string& path, float volume) {
    Sound* sound = loadSound(path);
    if (sound) {
        sound->setVolume(volume);
        sound->play();
    }
} 