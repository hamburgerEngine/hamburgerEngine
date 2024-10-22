#include "../../../include/gameName/states/PlayState.h"
#include "../../../include/engine/Engine.h"

PlayState::PlayState() : backgroundSprite(nullptr), playerSprite(nullptr) {}

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
    engine->addAnimatedSprite(playerSprite);
    playerSprite->playAnimation("idle");
}

void PlayState::update(float deltaTime) {
    playerSprite->update(deltaTime);
}

void PlayState::render() {
    backgroundSprite->render();
    playerSprite->render();
}

void PlayState::destroy() {
    delete backgroundSprite;
    delete playerSprite;
}
