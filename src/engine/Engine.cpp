#include "../../include/engine/Engine.h"
#include "../../include/engine/State.h"
#include "../../include/engine/SubState.h"
#include "../../include/engine/Input.h"
#include <iostream>
#include "../../include/engine/Sprite.h"
#include "../../include/engine/AnimatedSprite.h"
#include "../../include/engine/Text.h"
#include <algorithm>

Engine* Engine::instance = nullptr;

Engine::Engine(int width, int height, const char* title)
    : windowWidth(width), windowHeight(height) {
    if (instance == nullptr) {
        instance = this;
    }

    int argc = 1;
    char* argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow(title);

    glutDisplayFunc(displayCallback);
    glutIdleFunc(idleCallback);
    glutReshapeFunc(reshapeCallback);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutKeyboardFunc(Engine::keyboardCallback);
    glutKeyboardUpFunc(Engine::keyboardUpCallback);
    glutSpecialFunc(Engine::specialKeyCallback);
    glutSpecialUpFunc(Engine::specialKeyUpCallback);
}

Engine::~Engine() {
    for (auto sprite : sprites) {
        delete sprite;
    }
    for (auto animatedSprite : animatedSprites) {
        delete animatedSprite;
    }
    for (auto text : texts) {
        delete text;
    }
}

void Engine::run() {
    glutMainLoop();
}

void Engine::update() {
    static float lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    Input::getInstance().update();

    if (!states.empty()) {
        states.top()->update(deltaTime);
    }
}

void Engine::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (!states.empty()) {
        states.top()->render();
    }

    glutSwapBuffers();
}

void Engine::displayCallback() {
    instance->update();
    instance->render();
}

void Engine::idleCallback() {
    glutPostRedisplay();
}

void Engine::reshapeCallback(int width, int height) {
    instance->windowWidth = width;
    instance->windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

void Engine::pushState(State* state) {
    states.push(state);
    state->create();
}

void Engine::popState() {
    if (!states.empty()) {
        states.top()->destroy();
        delete states.top();
        states.pop();
    }
}

void Engine::switchState(State* state) {
    if (!states.empty()) {
        State* oldState = states.top();
        states.pop();
        oldState->destroy();
        delete oldState;
        
        sprites.clear();
        animatedSprites.clear();
        texts.clear();
    }
    pushState(state);
}

void Engine::keyboardCallback(unsigned char key, int x, int y) {
    Input::getInstance().keyPressed(key);
}

void Engine::keyboardUpCallback(unsigned char key, int x, int y) {
    Input::getInstance().keyReleased(key);
}

void Engine::openSubState(SubState* subState) {
    std::cout << "Engine::openSubState called" << std::endl;
    if (!states.empty()) {
        states.top()->openSubState(subState);
    } else {
        std::cout << "No states to open substate on" << std::endl;
    }
}

void Engine::specialKeyCallback(int key, int x, int y) {
    if (!instance->states.empty()) {
        instance->states.top()->specialKeyPressed(key, x, y);
    }
}

void Engine::specialKeyUpCallback(int key, int x, int y) {
    int mappedKey;
    switch(key) {
        case GLUT_KEY_UP:    mappedKey = 128; break;
        case GLUT_KEY_DOWN:  mappedKey = 129; break;
        case GLUT_KEY_LEFT:  mappedKey = 130; break;
        case GLUT_KEY_RIGHT: mappedKey = 131; break;
        default: return;
    }
    Input::getInstance().keyReleased(mappedKey);
}
