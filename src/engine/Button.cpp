#include "Button.h"
#include "SDLManager.h"

Button::Button(float x, float y, const std::string& text, std::function<void()> onClick)
    : Text(x, y), onClick(onClick), padding(10.0f), hovered(false), pressed(false) {
    setText(text);
    backgroundColor = 0x808080FF;
    hoverColor = 0xA0A0A0FF;
}

Button::~Button() {}

void Button::update(float deltaTime) {
    Text::update(deltaTime);
    checkHover();
    checkClick();
}

void Button::render() {
    SDL_Rect rect = {
        static_cast<int>(getX() - padding),
        static_cast<int>(getY() - padding),
        static_cast<int>(getWidth() + padding * 2),
        static_cast<int>(getHeight() + padding * 2)
    };

    unsigned int currentColor = hovered ? hoverColor : backgroundColor;
    SDL_SetRenderDrawColor(SDLManager::getInstance().getRenderer(),
                          (currentColor >> 24) & 0xFF,
                          (currentColor >> 16) & 0xFF,
                          (currentColor >> 8) & 0xFF,
                          currentColor & 0xFF);
    SDL_RenderFillRect(SDLManager::getInstance().getRenderer(), &rect);

    Text::render();
}

void Button::checkHover() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    hovered = (mouseX >= getX() - padding &&
               mouseX <= getX() + getWidth() + padding &&
               mouseY >= getY() - padding &&
               mouseY <= getY() + getHeight() + padding);
}

void Button::checkClick() {
    if (hovered && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (!pressed) {
            pressed = true;
            if (onClick) {
                onClick();
            }
        }
    } else {
        pressed = false;
    }
} 