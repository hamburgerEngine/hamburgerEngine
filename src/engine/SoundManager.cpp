#include "SoundManager.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

SoundManager::SoundManager() : currentMusic(nullptr) {
    ALCdevice* device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to open audio device" << std::endl;
        return;
    }

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!context) {
        std::cerr << "Failed to create audio context" << std::endl;
        alcCloseDevice(device);
        return;
    }

    if (!alcMakeContextCurrent(context)) {
        std::cerr << "Failed to make audio context current" << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);
        return;
    }
}

SoundManager::~SoundManager() {
    for (auto& pair : sounds) {
        delete pair.second;
    }
    if (currentMusic) {
        delete currentMusic;
    }

    ALCcontext* context = alcGetCurrentContext();
    if (context) {
        ALCdevice* device = alcGetContextsDevice(context);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        if (device) {
            alcCloseDevice(device);
        }
    }
}

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::playMusic(const std::string& path, float volume) {
    if (currentMusic) {
        currentMusic->stop();
        delete currentMusic;
    }

    currentMusic = new Sound();
    if (currentMusic->load(path)) {
        currentMusic->setLoop(true);
        currentMusic->setVolume(volume);
        currentMusic->play();
    }
}

void SoundManager::pauseMusic() {
    if (currentMusic) {
        currentMusic->pause();
    }
}

void SoundManager::resumeMusic() {
    if (currentMusic) {
        currentMusic->resume();
    }
}

void SoundManager::stopMusic() {
    if (currentMusic) {
        currentMusic->stop();
    }
}

void SoundManager::setMusicVolume(float volume) {
    if (currentMusic) {
        currentMusic->setVolume(volume);
    }
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