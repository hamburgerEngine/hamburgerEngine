#include "Sound.h"
#include <iostream>

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
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
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
        std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
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
