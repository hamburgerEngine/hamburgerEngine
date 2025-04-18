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
    Engine* engine = Engine::getInstance();
    
    videoPlayer = new VideoPlayer();
    if (videoPlayer->load("assets/videos/random_ass_alpha_3_run.mkv")) {
        videoPlayer->setWindowSize(engine->getWindowWidth(), engine->getWindowHeight());
        videoPlayer->setMaintainAspectRatio(false);
        videoPlayer->play();
        Log::getInstance().info("Video playback started");
    } else {
        Log::getInstance().error("Failed to load video");
    }
}

void VideoState::update(float deltaTime) {
    Input::UpdateKeyStates();
    Input::UpdateControllerStates();

    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        if (videoPlayer) {
            videoPlayer->update(deltaTime);
        }
    }
}

void VideoState::render() {
    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
    else {
        if (videoPlayer) {
            videoPlayer->render(SDLManager::getInstance().getRenderer());
        }
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