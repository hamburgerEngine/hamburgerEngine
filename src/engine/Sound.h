#pragma once
#include <string>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>

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
    OggVorbis_File vorbisFile;
    bool isLoaded;
    bool playing;
    bool looping;
    float volume;
    FILE* file;
    
    ALuint buffer;
    ALuint source;
}; 