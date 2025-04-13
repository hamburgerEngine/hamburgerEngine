#ifdef __MINGW32__  || defined(__SWITCH__)
#include "gameName/states/PlayState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "gameName/substates/PauseSubState.h"
#include "engine/SoundManager.h"
#else
#include <states/PlayState.h>
#include <Engine.h>
#include <Input.h>
#include <substates/PauseSubState.h>
#include <SoundManager.h>
#endif
#include <iostream>

PlayState* PlayState::instance = nullptr;

PlayState::PlayState() : backgroundSprite(nullptr), playerSprite(nullptr) {
    instance = this; // haxeflixel reference
}

PlayState::~PlayState() {
    destroy();
}

void PlayState::create() {
    Engine* engine = Engine::getInstance();

    backgroundSprite = new Sprite();
    backgroundSprite->loadTexture(Paths::image("background"));
    engine->addSprite(backgroundSprite);

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

    SoundManager::getInstance().playMusic(Paths::music("RunFNFInst"));
    Log::getInstance().info("PlayState initialized lol! Also a log test.");
}

void PlayState::update(float deltaTime) {
    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        playerSprite->update(deltaTime);
        
        if (Input::getInstance().isKeyJustPressed(SDLK_UP) || Input::getInstance().isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_UP)) {
            playerSprite->playAnimation("up");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Up pressed");
            #endif
        } else if (Input::getInstance().isKeyJustPressed(SDLK_DOWN) || Input::getInstance().isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
            playerSprite->playAnimation("down");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Down pressed");
            #endif
        } else if (Input::getInstance().isKeyJustPressed(SDLK_LEFT) || Input::getInstance().isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
            playerSprite->playAnimation("left");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Left pressed");
            #endif
        } else if (Input::getInstance().isKeyJustPressed(SDLK_RIGHT) || Input::getInstance().isControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
            playerSprite->playAnimation("right");
            #ifdef __SWITCH__ 
                Log::getInstance().info("Right pressed");
            #endif
        } else if (Input::getInstance().isControllerButtonPressed(SDL_CONTROLLER_BUTTON_A) || Input::getInstance().isKeyJustPressed(SDLK_SPACE)) {
            playerSprite->playAnimation("hey");
            #ifdef __SWITCH__ 
                Log::getInstance().info("A pressed");
            #endif
        } else {
            playerSprite->playAnimation("idle");
        }

        if (Input::getInstance().isKeyJustPressed(SDLK_RETURN) || Input::getInstance().isControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_START)) {
            if (_subStates.empty()) {
                PauseSubState* pauseSubState = new PauseSubState();
                openSubState(pauseSubState);
                Log::getInstance().info("Pause SubState opened");
            } else {
                closeSubState();
                Log::getInstance().info("Pause SubState closed");
            }
        }

        if (Input::getInstance().isKeyJustPressed(SDLK_r) || Input::getInstance().isControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_B)) {
            playerSprite->playAnimation("idle");
            #ifdef __SWITCH__ 
                Log::getInstance().info("B pressed");
            #endif
        }
    }
}

void PlayState::render() {
    backgroundSprite->render();
    playerSprite->render();

    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
}

void PlayState::destroy() {
    delete backgroundSprite;
    delete playerSprite;

    backgroundSprite = nullptr;
    playerSprite = nullptr;
}

void PlayState::openSubState(SubState* subState) {
    std::cout << "PlayState::openSubState called" << std::endl;
    State::openSubState(subState);
}