#include "../../include/engine/Engine.h"
#include <iostream>
#include "../../include/engine/Sprite.h"
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
}

Engine::~Engine() {
    for (auto sprite : sprites) {
        delete sprite;
    }
    for (auto text : texts) {
        delete text;
    }
}

void Engine::run() {
    glutMainLoop();
}

void Engine::update() {
    for (auto sprite : sprites) {
        sprite->update();
    }
}

void Engine::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    for (auto sprite : sprites) {
        sprite->render();
    }
    
    for (auto text : texts) {
        text->render();
    }

    glutSwapBuffers();
}

void Engine::displayCallback() {
    instance->render();
}

void Engine::idleCallback() {
    instance->update();
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
