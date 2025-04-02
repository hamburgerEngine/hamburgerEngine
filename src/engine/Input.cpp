#ifdef __MINGW32__
#include "engine/Input.h"
#else
#include <Input.h>
#endif
#include <iostream>

Input::Input() {
    currentKeyState.fill(false);
    previousKeyState.fill(false);
    justPressedState.fill(false);
    justReleasedState.fill(false);
}

void Input::update() {
    previousKeyState = currentKeyState;
    justPressedState.fill(false);
    justReleasedState.fill(false);
}

void Input::keyPressed(unsigned char key) {
    if (isValidKey(key)) {
        if (!currentKeyState[key]) {
            currentKeyState[key] = true;
            justPressedState[key] = true;
        }
    }
}

void Input::specialKeyPressed(int key) {
    if (isValidKey(key)) {
        if (!currentKeyState[key]) {
            currentKeyState[key] = true;
            justPressedState[key] = true;
        }
    }
}

void Input::keyReleased(unsigned char key) {
    if (isValidKey(key)) {
        currentKeyState[key] = false;
        justReleasedState[key] = true;
    }
}

void Input::specialKeyReleased(int key) {
    if (isValidKey(key)) {
        currentKeyState[key] = false;
        justReleasedState[key] = true;
    }
}

bool Input::isKeyPressed(int key) {
    return isValidKey(key) && currentKeyState[key];
}

bool Input::isKeyJustPressed(int key) {
    return isValidKey(key) && justPressedState[key];
}

bool Input::isKeyJustReleased(int key) {
    return isValidKey(key) && justReleasedState[key];
}
