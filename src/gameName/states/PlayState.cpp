#ifdef __MINGW32__  || defined(__SWITCH__)
#include "gameName/states/PlayState.h"
#include "engine/core/Engine.h"
#include "engine/input/Input.h"
#include "gameName/substates/PauseSubState.h"
#include "engine/audio/SoundManager.h"
#else
#include <states/PlayState.h>
#include <core/Engine.h>
#include <input/Input.h>
#include <substates/PauseSubState.h>
#include <audio/SoundManager.h>
#endif
#include <iostream>

PlayState* PlayState::instance = nullptr;

PlayState::PlayState() : playerSprite(nullptr), button(nullptr) {
    instance = this; // haxeflixel reference
}

PlayState::~PlayState() {
    destroy();
}

void PlayState::create() {
    Engine* engine = Engine::getInstance(); 
    
    playerSprite = new AnimatedSprite();
    playerSprite->setPosition(100, 100);
    playerSprite->loadFrames(Paths::image("BOYFRIEND"), Paths::xml("images/BOYFRIEND"));
    
    playerSprite->addAnimation("idle", "BF idle dance0", 24, true);
    playerSprite->addAnimation("up", "BF NOTE UP0", 24, true);
    playerSprite->addAnimation("down", "BF NOTE DOWN0", 24, true);
    playerSprite->addAnimation("left", "BF NOTE LEFT0", 24, true);   
    playerSprite->addAnimation("right", "BF NOTE RIGHT0", 24, true);
    playerSprite->addAnimation("hey", "BF HEY!!", 24, true);
    
    engine->addAnimatedSprite(playerSprite);
    playerSprite->playAnimation("idle");

    button = new Button(400, 100, "Click Me", []() {
        Log::getInstance().info("Button clicked!");
    });

    button->setBackgroundColor(0xFF0000FF);
    button->setHoverColor(0xFF8080FF);
    button->setPadding(15.0f);
    button->setFormat("assets/fonts/vcr.ttf", 24, 0xFFFFFFFF);
    button->setVisible(true);

    SoundManager::getInstance().playMusic(Paths::music("RunFNFInst"));
    Log::getInstance().info("PlayState initialized lol! Also a log test.");
}

void PlayState::update(float deltaTime) {
    Input::UpdateKeyStates();
    Input::UpdateControllerStates();

    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        playerSprite->update(deltaTime);
        button->update(deltaTime);
        
        if (Input::pressed(SDL_SCANCODE_UP) || Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_UP)) {
            playerSprite->playAnimation("up");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Up pressed");
            #endif
        } else if (Input::pressed(SDL_SCANCODE_DOWN) || Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
            playerSprite->playAnimation("down");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Down pressed");
            #endif
        } else if (Input::pressed(SDL_SCANCODE_LEFT) || Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
            playerSprite->playAnimation("left");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Left pressed");
            #endif
        } else if (Input::pressed(SDL_SCANCODE_RIGHT) || Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
            playerSprite->playAnimation("right");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Right pressed");
            #endif
        } else if (Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_A) || Input::pressed(SDL_SCANCODE_SPACE)) {
            playerSprite->playAnimation("hey");
            #ifdef __SWITCH__ 
                Log::getInstance().info("A pressed");
            #endif
        } else {
            playerSprite->playAnimation("idle");
        }

        if (Input::justPressed(SDL_SCANCODE_RETURN) || Input::isControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_START)) {
            if (_subStates.empty()) {
                PauseSubState* pauseSubState = new PauseSubState();
                openSubState(pauseSubState);
                Log::getInstance().info("Pause SubState opened");
            } else {
                closeSubState();
                Log::getInstance().info("Pause SubState closed");
            }
        }

        if (Input::pressed(SDL_SCANCODE_R) || Input::isControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_B)) {
            playerSprite->playAnimation("idle");
            #ifdef __SWITCH__ 
                Log::getInstance().info("B pressed");
            #endif
        }
    }
}

void PlayState::render() {
    playerSprite->render();

    if (!_subStates.empty()) {
        _subStates.back()->render();
    }

    button->render();
}

void PlayState::destroy() {
    delete playerSprite;
    delete button;
    
    playerSprite = nullptr;
    button = nullptr;
}

void PlayState::openSubState(SubState* subState) {
    State::openSubState(subState);
}