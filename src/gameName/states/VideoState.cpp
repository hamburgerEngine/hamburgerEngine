#ifdef __MINGW32__  || defined(__SWITCH__)
#include "gameName/states/VideoState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "gameName/substates/PauseSubState.h"
#include "engine/SoundManager.h"
#include "engine/Log.h"
#else
#include <states/VideoState.h>
#include <Engine.h>
#include <Input.h>
#include <substates/PauseSubState.h>
#include <SoundManager.h>
#include <Log.h>
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