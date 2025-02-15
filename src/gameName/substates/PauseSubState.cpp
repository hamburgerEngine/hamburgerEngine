#include <Engine.h>
#include <Input.h>
#include <substates/PauseSubState.h>
#include <iostream>

PauseSubState::PauseSubState() : pauseText(nullptr) {
}

PauseSubState::~PauseSubState() {
    destroy();
}

void PauseSubState::create() {
    pauseText = new Text(300, 250, 200);
    pauseText->setText("PAUSED");
    pauseText->setFormat("assets/fonts/Zero G.ttf", 36, 0xFFFFFFFF);
}

void PauseSubState::update(float deltaTime) {
    if (Input::getInstance().isKeyJustPressed('p')) {
        std::cout << "P key pressed in PauseSubState, closing" << std::endl;
        getParentState()->closeSubState();
    }
}

void PauseSubState::render() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    pauseText->render();
}

void PauseSubState::destroy() {
    if (pauseText) {
        delete pauseText;
        pauseText = nullptr;
    }
}
