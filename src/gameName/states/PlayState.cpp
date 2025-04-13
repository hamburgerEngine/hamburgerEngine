#ifdef __MINGW32__
#include "gameName/states/PlayState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "gameName/substates/PauseSubState.h"
#include "engine/SoundManager.h"
#elif defined(__SWITCH__)
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

    backgroundSprite = new Sprite("assets/images/background.png");
    engine->addSprite(backgroundSprite);

    playerSprite = new AnimatedSprite();
    playerSprite->setPosition(100, 100);
    playerSprite->loadFrames("assets/images/BOYFRIEND.png", "assets/images/BOYFRIEND.xml");
    
    playerSprite->addAnimation("idle", "BF idle dance0", 24, true);
    playerSprite->addAnimation("up", "BF NOTE UP0", 24, true);
    playerSprite->addAnimation("down", "BF NOTE DOWN0", 24, true);
    playerSprite->addAnimation("left", "BF NOTE LEFT0", 24, true);   
    playerSprite->addAnimation("right", "BF NOTE RIGHT0", 24, true);
    
    engine->addAnimatedSprite(playerSprite);
    playerSprite->playAnimation("idle");

    SoundManager::getInstance().playMusic("assets/music/RunFNFInst.ogg");
}

void PlayState::update(float deltaTime) {
    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        playerSprite->update(deltaTime);
        
        if (Input::getInstance().isKeyPressed(SDLK_UP)) {
            playerSprite->playAnimation("up");
        } else if (Input::getInstance().isKeyPressed(SDLK_DOWN)) {
            playerSprite->playAnimation("down");
        } else if (Input::getInstance().isKeyPressed(SDLK_LEFT)) {
            playerSprite->playAnimation("left");
        } else if (Input::getInstance().isKeyPressed(SDLK_RIGHT)) {
            playerSprite->playAnimation("right");
        } else {
            playerSprite->playAnimation("idle");
        }

        if (Input::getInstance().isKeyJustPressed(SDLK_p)) {
            if (_subStates.empty()) {
                PauseSubState* pauseSubState = new PauseSubState();
                openSubState(pauseSubState);
            } else {
                closeSubState();
            }
        }

        if (Input::getInstance().isKeyJustPressed(SDLK_r)) {
            playerSprite->playAnimation("idle");
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