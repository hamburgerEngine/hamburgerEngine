# State

```cpp
// StateName.cpp
#ifdef __MINGW32__
#include "gameName/states/PlayState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "gameName/substates/PauseSubState.h"
#else
#include <states/PlayState.h>
#include <Engine.h>
#include <Input.h>
#include <substates/PauseSubState.h>
#endif
#include <iostream>

StateName* StateName::instance = nullptr;

StateName::StateName() : backgroundSprite(nullptr), playerSprite(nullptr) {
    instance = this; // haxeflixel reference
}

StateName::~StateName() {
    destroy();
}

void StateName::create() {
    Engine* engine = Engine::getInstance();
}

void StateName::update(float deltaTime) {
    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    }
}

void StateName::render() {
    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
}

void StateName::destroy() {
}

void StateName::openSubState(SubState* subState) {
    std::cout << "StateName::openSubState called" << std::endl;
    State::openSubState(subState);
}

void StateName::keyPressed(unsigned char key, int x, int y) {
}

void StateName::specialKeyPressed(int key, int x, int y) {
}
```

```cpp
// StateName.h
#pragma once
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"

void stateNameKeyboardCallback(unsigned char key, int x, int y);

class StateName : public State {
public:
    StateName();
    ~StateName();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

    void openSubState(SubState* subState);

    virtual void keyPressed(unsigned char key, int x, int y) override;
    virtual void specialKeyPressed(int key, int x, int y) override;
    static StateName* instance;

private:
};
```
