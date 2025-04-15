#ifdef __MINGW32__  || defined(__SWITCH__)
#include "gameName/states/BaseState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "gameName/substates/PauseSubState.h"
#include "engine/SoundManager.h"
#else
#include <states/BaseState.h>
#include <Engine.h>
#include <Input.h>
#include <substates/PauseSubState.h>
#include <SoundManager.h>
#endif
#include <iostream>

BaseState* BaseState::instance = nullptr;

BaseState::BaseState() {
    instance = this; // haxeflixel reference
}

BaseState::~BaseState() {
    destroy();
}

void BaseState::create() {
    Engine* engine = Engine::getInstance();
    Log::getInstance().info("BaseState initialized lol! Also a log test.");
}

void BaseState::update(float deltaTime) {
    Input::UpdateKeyStates();
    Input::UpdateControllerStates();

    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
    }
}

void BaseState::render() {
    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
}

void BaseState::destroy() {
}

void BaseState::openSubState(SubState* subState) {
    State::openSubState(subState);
}