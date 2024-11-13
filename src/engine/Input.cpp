#include "../../include/engine/Input.h"
#include <iostream>
#include <functional>
#include <cctype>

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

void Input::setCallback(const std::string& action, std::function<void()> callback) {
    callbacks[action] = callback;
}

void Input::keyPressed(unsigned char key) {
    if (isValidKey(key)) {
        if (!currentKeyState[key]) {
            currentKeyState[key] = true;
            justPressedState[key] = true;
            handleKeyPress(key);
        }
    }
}

void Input::keyReleased(unsigned char key) {
    if (isValidKey(key)) {
        if (currentKeyState[key]) {
            currentKeyState[key] = false;
            justReleasedState[key] = true;
            handleKeyRelease(key);
        }
    }
}

bool Input::isKeyPressed(unsigned char key) {
    return isValidKey(key) && currentKeyState[key];
}

bool Input::isKeyJustPressed(unsigned char key) {
    return isValidKey(key) && justPressedState[key];
}

bool Input::isKeyJustReleased(unsigned char key) {
    return isValidKey(key) && justReleasedState[key];
}

void Input::handleKeyPress(unsigned char key) {
    std::string action;

    if (std::isalpha(key)) {
        action = "press_" + std::string(1, std::tolower(key));
    } else if (std::isdigit(key)) {
        action = "press_" + std::string(1, key);
    } else {
        switch (key) {
            case ' ': action = "press_space"; break;
            case 13: action = "press_enter"; break;
            case 9: action = "press_tab"; break;
            case 16: action = "press_shift"; break;
            default: return;
        }
    }

    if (callbacks.count(action) > 0) {
        callbacks[action]();
    }
}

void Input::handleKeyRelease(unsigned char key) {
    std::string action;

    if (std::isalpha(key)) {
        action = "release_" + std::string(1, std::tolower(key));
    } else if (std::isdigit(key)) {
        action = "release_" + std::string(1, key);
    } else {
        switch (key) {
            case ' ': action = "release_space"; break;
            case 13: action = "release_enter"; break;
            case 9: action = "release_tab"; break;
            case 16: action = "release_shift"; break;
            default: return;
        }
    }

    if (callbacks.count(action) > 0) {
        callbacks[action]();
    }
}

bool Input::isValidKey(unsigned char key) {
    return std::isalpha(key) || std::isdigit(key) || 
           key == ' ' || key == 13 || key == 9 || key == 16;
}
