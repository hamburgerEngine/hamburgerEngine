#ifdef __MINGW32__  || defined(__SWITCH__)
#include "SplashState.h"
#include "../core/Engine.h"
#include "../input/Input.h"
#include "../audio/SoundManager.h"
#include "gameName/states/PlayState.h"
#include "../core/SDLManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#else
#include <states/SplashState.h>
#include <core/Engine.h>
#include <input/Input.h>
#include <audio/SoundManager.h>
#include <states/PlayState.h>
#include <core/SDLManager.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#endif
#include <iostream>

SplashState* SplashState::instance = nullptr;

SplashState::SplashState() {
    instance = this; // haxeflixel reference
}

SplashState::~SplashState() {
    destroy();
}

void SplashState::create() {
    Engine* engine = Engine::getInstance();
    alpha = 0.0f;
    
    logoSprite = new Sprite(Paths::image("../splash"));
    if (logoSprite) {
        SDL_Window* window = SDLManager::getInstance().getWindow();
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        logoSprite->setPosition(
            (screenWidth - logoSprite->getWidth()) / 2,
            (screenHeight - logoSprite->getHeight()) / 2
        );
    }
    
    fadeIn(2.0f);
    
    engine->setTimeout([this]() { 
        this->fadeOut(1.5f); 
    }, 5.0f);
}

void SplashState::update(float deltaTime) {
    Input::UpdateKeyStates();
    Input::UpdateControllerStates();
    
    if (fadeInProgress > 0.0f) {
        fadeInProgress -= deltaTime;
        if (fadeInProgress <= 0.0f) {
            fadeInProgress = 0.0f;
            alpha = 1.0f;
        } else {
            alpha = 1.0f - (fadeInProgress / fadeInDuration);
        }
    }
    
    if (fadeOutProgress > 0.0f) {
        fadeOutProgress -= deltaTime;
        if (fadeOutProgress <= 0.0f) {
            fadeOutProgress = 0.0f;
            alpha = 0.0f;
            
            PlayState* newState = new PlayState();
            
            destroy();
            
            Engine::getInstance()->switchState(newState);
            
            return;
        } else {
            alpha = fadeOutProgress / fadeOutDuration;
        }
    }
    
    if (logoSprite) {
        logoSprite->setAlpha(alpha);
    }
}

void SplashState::render() {
    if (logoSprite) {
        logoSprite->render();
    }
}

void SplashState::destroy() {
    if (logoSprite) {
        delete logoSprite;
        logoSprite = nullptr;
    }
    
    alpha = 0.0f;
    fadeInProgress = 0.0f;
    fadeOutProgress = 0.0f;
    fadeInDuration = 0.0f;
    fadeOutDuration = 0.0f;
    
    for (auto subState : _subStates) {
        if (subState) {
            delete subState;
        }
    }
    _subStates.clear();
}

void SplashState::openSubState(SubState* subState) {
    State::openSubState(subState);
}

void SplashState::fadeIn(float duration) {
    fadeInDuration = duration;
    fadeInProgress = duration;
    fadeOutProgress = 0.0f;
}

void SplashState::fadeOut(float duration) {
    fadeOutDuration = duration;
    fadeOutProgress = duration;
    fadeInProgress = 0.0f;
}