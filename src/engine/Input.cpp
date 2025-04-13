#ifdef __MINGW32__
#include "Input.h"
#elif defined(__SWITCH__)
#include "Input.h"
#else
#include <Input.h>
#endif
#include <iostream>

Input::Input() : mouseX(0), mouseY(0), mouseDeltaX(0), mouseDeltaY(0) {}

Input::~Input() {}

void Input::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            if (!event.key.repeat) {
                currentKeyState[event.key.keysym.sym] = true;
                justPressedState[event.key.keysym.sym] = true;
            }
            break;
            
        case SDL_KEYUP:
            currentKeyState[event.key.keysym.sym] = false;
            justReleasedState[event.key.keysym.sym] = true;
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            currentMouseState[event.button.button] = true;
            justPressedMouseState[event.button.button] = true;
            break;
            
        case SDL_MOUSEBUTTONUP:
            currentMouseState[event.button.button] = false;
            justReleasedMouseState[event.button.button] = true;
            break;
            
        case SDL_MOUSEMOTION:
            mouseDeltaX = event.motion.xrel;
            mouseDeltaY = event.motion.yrel;
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
    }
}

void Input::update() {
    for (auto& pair : currentKeyState) {
        previousKeyState[pair.first] = pair.second;
    }
    justPressedState.clear();
    justReleasedState.clear();
    
    for (auto& pair : currentMouseState) {
        previousMouseState[pair.first] = pair.second;
    }
    justPressedMouseState.clear();
    justReleasedMouseState.clear();
    
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}

bool Input::isKeyPressed(SDL_Keycode key) const {
    auto it = currentKeyState.find(key);
    return it != currentKeyState.end() && it->second;
}

bool Input::isKeyJustPressed(SDL_Keycode key) const {
    auto it = justPressedState.find(key);
    return it != justPressedState.end() && it->second;
}

bool Input::isKeyJustReleased(SDL_Keycode key) const {
    auto it = justReleasedState.find(key);
    return it != justReleasedState.end() && it->second;
}

bool Input::isMouseButtonPressed(Uint8 button) const {
    auto it = currentMouseState.find(button);
    return it != currentMouseState.end() && it->second;
}

bool Input::isMouseButtonJustPressed(Uint8 button) const {
    auto it = justPressedMouseState.find(button);
    return it != justPressedMouseState.end() && it->second;
}

bool Input::isMouseButtonJustReleased(Uint8 button) const {
    auto it = justReleasedMouseState.find(button);
    return it != justReleasedMouseState.end() && it->second;
}

void Input::setCallback(const std::string& action, std::function<void()> callback) {
    callbacks[action] = callback;
}
