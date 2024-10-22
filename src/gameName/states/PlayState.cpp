#include "../../../include/gameName/states/PlayState.h"
#include "../../../include/engine/Engine.h"
#include "../../../include/gameName/substates/PauseSubState.h"
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
    playerSprite->addAnimation("idle", "BF idle dance", 24, true);
    playerSprite->addAnimation("up", "BF NOTE UP0", 24, false);
    playerSprite->addAnimation("down", "BF NOTE DOWN0", 24, true);
    playerSprite->addAnimation("left", "BF NOTE LEFT0", 24, true);   
    playerSprite->addAnimation("right", "BF NOTE RIGHT0", 24, true);
    engine->addAnimatedSprite(playerSprite);
    playerSprite->playAnimation("idle");
}

void PlayState::update(float deltaTime) {
    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        playerSprite->update(deltaTime);
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
}

void PlayState::openSubState(SubState* subState) {
    std::cout << "PlayState::openSubState called" << std::endl;
    State::openSubState(subState);
}

void PlayState::keyPressed(unsigned char key, int x, int y) {
    if (key == 'p') {
        if (instance->_subStates.empty()) {
            PauseSubState* pauseSubState = new PauseSubState();
            instance->openSubState(pauseSubState);
        } else {
            instance->closeSubState();
        }
    }

    if (key == 'r')
    {
        instance->playerSprite->playAnimation("idle");
    }

}

void PlayState::specialKeyPressed(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            std::cout << "Up arrow pressed" << std::endl;
            instance->playerSprite->playAnimation("up");
            break;
        case GLUT_KEY_DOWN:
            std::cout << "Down arrow pressed" << std::endl;
            instance->playerSprite->playAnimation("down");
            break;
        case GLUT_KEY_LEFT:
            std::cout << "Left arrow pressed" << std::endl;
            instance->playerSprite->playAnimation("left");
            break;
        case GLUT_KEY_RIGHT:
            std::cout << "Right arrow pressed" << std::endl;
            instance->playerSprite->playAnimation("right");
            break;
    }
}