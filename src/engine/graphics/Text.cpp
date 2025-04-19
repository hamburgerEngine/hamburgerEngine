#ifdef __MINGW32__ || defined(__SWITCH__)
#include "Text.h"
#include "../core/SDLManager.h"
#include <iostream>
#include <sstream>
#else
#include <graphics/Text.h>
#include <core/SDLManager.h>
#include <iostream>
#include <sstream>
#endif

Text::Text(float x, float y, int z) 
    : x(x), y(y), width(0), height(0), text(""), color(0xFFFFFFFF),
      fontSize(12), font(nullptr), texture(nullptr), isVisible(true),
      lineHeight(0), lineSpacing(1.2f) {
}

Text::~Text() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void Text::setText(const std::string& text) {
    this->text = text;
    updateTexture();
}

void Text::setFormat(const std::string& fontPath, int fontSize, unsigned int color) {
    this->fontSize = fontSize;
    this->color = color;
    loadFont(fontPath);
    updateTexture();
}

void Text::loadFont(const std::string& fontPath) {
    if (font) {
        TTF_CloseFont(font);
    }
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        Log::getInstance().error("Failed to load font: " + std::string(TTF_GetError()));
        return;
    }
    
    int fontHeight = TTF_FontHeight(font);
    lineHeight = static_cast<float>(fontHeight);
}

void Text::updateTexture() {
    if (!font || text.empty()) return;

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    std::istringstream iss(text);
    std::string line;
    float maxWidth = 0;
    int lineCount = 0;
    
    while (std::getline(iss, line)) {
        int w, h;
        TTF_SizeText(font, line.c_str(), &w, &h);
        maxWidth = std::max(maxWidth, static_cast<float>(w));
        lineCount++;
    }
    
    width = maxWidth;
    height = static_cast<float>(lineCount) * lineHeight * lineSpacing;
}

void Text::renderText(const std::string& text, float x, float y) {
    SDL_Color sdlColor = {
        static_cast<Uint8>((color >> 24) & 0xFF),
        static_cast<Uint8>((color >> 16) & 0xFF),
        static_cast<Uint8>((color >> 8) & 0xFF),
        static_cast<Uint8>(color & 0xFF)
    };

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
    if (!surface) {
        Log::getInstance().error("Failed to create text surface: " + std::string(TTF_GetError()));
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(SDLManager::getInstance().getRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!textTexture) {
        Log::getInstance().error("Failed to create texture from surface: " + std::string(SDL_GetError()));
        return;
    }

    SDL_Rect destRect;
    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);
    SDL_QueryTexture(textTexture, nullptr, nullptr, &destRect.w, &destRect.h);
    
    SDL_RenderCopy(SDLManager::getInstance().getRenderer(), textTexture, nullptr, &destRect);
    SDL_DestroyTexture(textTexture);
}

void Text::render() {
    if (!isVisible || !font) return;
    
    std::istringstream iss(text);
    std::string line;
    float currentY = y;
    
    while (std::getline(iss, line)) {
        renderText(line, x, currentY);
        currentY += lineHeight * lineSpacing;
    }
}

void Text::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void Text::update(float deltaTime) {}
