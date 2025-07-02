#ifdef __MINGW32__  || defined(__SWITCH__)
#include "gameName/states/VideoState.h"
#include "engine/core/Engine.h"
#include "engine/input/Input.h"
#include "gameName/substates/PauseSubState.h"
#include "engine/audio/SoundManager.h"
#include "engine/utils/Log.h"
#else
#include <states/VideoState.h>
#include <core/Engine.h>
#include <input/Input.h>
#include <substates/PauseSubState.h>
#include <audio/SoundManager.h>
#include <utils/Log.h>
#endif
#include <iostream>

VideoState* VideoState::instance = nullptr;

VideoState::VideoState() : videoPlayer(nullptr) {
    instance = this;
}

VideoState::~VideoState() {
    destroy();
}

void VideoState::create() {
    videoPlayer = new VideoPlayer();
    VideoState* videoState = VideoState::instance;
    if (videoState->loadVideo("assets/videos/bakurestu.mp4")) {
        videoState->setVolume(100);
        videoState->playVideo();
    }
}

void VideoState::update(float deltaTime) {
    Input::UpdateKeyStates();
    Input::UpdateControllerStates();

    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        if (videoPlayer) {
            videoPlayer->update();

            if (Input::justPressed(SDL_SCANCODE_SPACE)) {
                if (videoPlayer->isPlaying()) {
                    videoPlayer->pause();
                } else {
                    videoPlayer->play();
                }
            }

            if (Input::justPressed(SDL_SCANCODE_ESCAPE)) {
                videoPlayer->stop();
            }
        }
    }
}

void VideoState::render() {
    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
    if (videoPlayer) {
        videoPlayer->render(SDLManager::getInstance().getRenderer());
    }
}

void VideoState::destroy() {
    if (videoPlayer) {
        delete videoPlayer;
        videoPlayer = nullptr;
    }
}

void VideoState::openSubState(SubState* subState) {
    State::openSubState(subState);
}

bool VideoState::loadVideo(const std::string& path) {
    if (!videoPlayer) {
        Log::getInstance().error("Video player not initialized");
        return false;
    }

    currentVideoPath = path;
    return videoPlayer->loadVideo(path);
}

void VideoState::playVideo() {
    if (videoPlayer) {
        videoPlayer->play();
    }
}

void VideoState::pauseVideo() {
    if (videoPlayer) {
        videoPlayer->pause();
    }
}

void VideoState::stopVideo() {
    if (videoPlayer) {
        videoPlayer->stop();
    }
}

void VideoState::setVolume(int volume) {
    if (videoPlayer) {
        videoPlayer->setVolume(volume);
    }
}
