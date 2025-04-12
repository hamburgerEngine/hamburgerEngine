#define _CRT_SECURE_NO_WARNINGS
#include "Sound.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <vector>

#define BUFFER_SIZE 32768  // 32 KB buffer :P

Sound::Sound() : isLoaded(false), playing(false), looping(false), volume(1.0f), file(nullptr) {
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);
}

Sound::~Sound() {
    if (isLoaded) {
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
        ov_clear(&vorbisFile);
    }
    if (file) {
        fclose(file);
    }
}

bool Sound::load(const std::string& path) {
    file = fopen(path.c_str(), "rb");
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    if (ov_open_callbacks(file, &vorbisFile, nullptr, 0, OV_CALLBACKS_DEFAULT) < 0) {
        std::cerr << "Failed to open Ogg file: " << path << std::endl;
        fclose(file);
        return false;
    }

    vorbis_info* vorbisInfo = ov_info(&vorbisFile, -1);
    ALenum format = (vorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    std::vector<char> buffer_data;
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];
    
    do {
        bytes = ov_read(&vorbisFile, array, BUFFER_SIZE, 0, 2, 1, &bitStream);
        if (bytes > 0) {
            buffer_data.insert(buffer_data.end(), array, array + bytes);
        }
    } while (bytes > 0);

    alBufferData(buffer, format, buffer_data.data(), buffer_data.size(), vorbisInfo->rate);

    alSourcei(source, AL_BUFFER, buffer);
    alSourcef(source, AL_GAIN, volume);
    alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);

    isLoaded = true;
    return true;
}

void Sound::play() {
    if (!isLoaded) return;
    alSourcePlay(source);
    playing = true;
}

void Sound::pause() {
    if (!isLoaded) return;
    alSourcePause(source);
    playing = false;
}

void Sound::resume() {
    if (!isLoaded) return;
    alSourcePlay(source);
    playing = true;
}

void Sound::stop() {
    if (!isLoaded) return;
    alSourceStop(source);
    playing = false;
}

void Sound::setVolume(float vol) {
    volume = vol;
    if (isLoaded) {
        alSourcef(source, AL_GAIN, volume);
    }
}

void Sound::setLoop(bool loop) {
    looping = loop;
    if (isLoaded) {
        alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
    }
}

bool Sound::isPlaying() const {
    if (!isLoaded) return false;
    
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}
