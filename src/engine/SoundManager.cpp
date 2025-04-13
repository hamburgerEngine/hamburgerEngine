#include "SoundManager.h"
#include <iostream>

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
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }

    Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
    if (Mix_PlayMusic(currentMusic, -1) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
    }
}

void SoundManager::loopMusic(const std::string& path, float volume, int loops) {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
    }

    currentMusic = Mix_LoadMUS(path.c_str());
    if (!currentMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }

    Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
    if (Mix_PlayMusic(currentMusic, loops) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
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