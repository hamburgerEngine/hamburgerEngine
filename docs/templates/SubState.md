# SubState

```cpp
// SubStateName.h
#pragma once
#include <engine/SubState.h>
#include <engine/Text.h>

class SubStateName : public SubState {
public:
    SubStateName();
    ~SubStateName();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

private:
    Text* pauseText;
};
```

```cpp
// SubStateName.cpp
#include <substates/SubStateName.h>
#include <engine/Engine.h>
#include <engine/Input.h>
#include <iostream>

SubStateName::SubStateName() : pauseText(nullptr) {
}

SubStateName::~SubStateName() {
    destroy();
}

void SubStateName::create() {
}

void SubStateName::update(float deltaTime) {
}

void SubStateName::render() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    pauseText->render();
}

void SubStateName::destroy() {
}
```
