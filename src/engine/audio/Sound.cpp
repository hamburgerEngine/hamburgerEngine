#include "Sound.h"
#include <iostream>
#include "../utils/Log.h"

Sound::Sound() : sound(nullptr), isLoaded(false), playing(false), looping(false), volume(1.0f), channel(-1) {
}

Sound::~Sound() {
    if (isLoaded) {
        Mix_FreeChunk(sound);
    }
}

bool Sound::load(const std::string& path) {
    sound = Mix_LoadWAV(path.c_str());
    if (!sound) {
        Log::getInstance().error("Failed to load sound: " + std::string(Mix_GetError()));
        return false;
    }

    isLoaded = true;
    return true;
}

void Sound::play() {
    if (!isLoaded) return;
    
    int loops = looping ? -1 : 0;
    channel = Mix_PlayChannel(-1, sound, loops);
    if (channel == -1) {
        Log::getInstance().error("Failed to play sound: " + std::string(Mix_GetError()));
        return;
    }
    
    Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));
    playing = true;
}

void Sound::pause() {
    if (!isLoaded || channel == -1) return;
    Mix_Pause(channel);
    playing = false;
}

void Sound::resume() {
    if (!isLoaded || channel == -1) return;
    Mix_Resume(channel);
    playing = true;
}

void Sound::stop() {
    if (!isLoaded || channel == -1) return;
    Mix_HaltChannel(channel);
    playing = false;
    channel = -1;
}

void Sound::setVolume(float vol) {
    volume = vol;
    if (isLoaded && channel != -1) {
        Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));
    }
}

void Sound::setLoop(bool loop) {
    looping = loop;
    if (isLoaded && playing && channel != -1) {
        int loops = looping ? -1 : 0;
        Mix_PlayChannel(channel, sound, loops);
    }
}

bool Sound::isPlaying() const {
    if (!isLoaded || channel == -1) return false;
    return Mix_Playing(channel) && !Mix_Paused(channel);
}
