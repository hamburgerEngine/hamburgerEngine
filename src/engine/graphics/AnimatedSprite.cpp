#ifdef __MINGW32__ || defined(__SWITCH__)
#include "AnimatedSprite.h"
#include "../core/SDLManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#else
#include <graphics/AnimatedSprite.h>
#include <core/SDLManager.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#endif

AnimatedSprite::AnimatedSprite() : Sprite() {}

AnimatedSprite::AnimatedSprite(const std::string& path) : Sprite(path) {}

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::update(float deltaTime) {
    if (!currentAnimation || !visible) {
        return;
    }

    frameTimer += deltaTime;
    float frameDuration = 1.0f / currentAnimation->frameRate;
    
    if (frameTimer >= frameDuration) {
        currentFrame++;
        if (currentFrame >= currentAnimation->frames.size()) {
            if (currentAnimation->loop) {
                currentFrame = 0;
            } else {
                currentFrame = currentAnimation->frames.size() - 1;
                if (onAnimationFinished) {
                    onAnimationFinished();
                    onAnimationFinished = nullptr;
                }
            }
        }
        frameTimer -= frameDuration;
    }
}

void AnimatedSprite::render() {
    if (!visible || !currentAnimation || currentAnimation->frames.empty()) {
        if (!currentAnimation) Log::getInstance().error("No current animation");
        if (currentAnimation && currentAnimation->frames.empty()) Log::getInstance().error("No frames in animation");
        return;
    }

    const Frame& frame = currentAnimation->frames[currentFrame];
    
    SDL_Rect srcRect = {
        frame.x,
        frame.y,
        frame.width,
        frame.height
    };

    SDL_Rect destRect = {
        static_cast<int>(x + offsetX),
        static_cast<int>(y + offsetY),
        static_cast<int>(frame.width * scale.x),
        static_cast<int>(frame.height * scale.y)
    };

    SDL_RendererFlip flip = scale.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(SDLManager::getInstance().getRenderer(), texture, &srcRect, &destRect, 0, nullptr, flip);
}

void AnimatedSprite::loadTexture(const std::string& imagePath) {
    if (texture) {
        Log::getInstance().info("Texture already loaded, skipping.");
        return;
    }

    Log::getInstance().info("Attempting to load image from: " + imagePath);
    
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        Log::getInstance().error("Failed to load image: " + imagePath);
        Log::getInstance().error("SDL_image error: " + std::string(IMG_GetError()));
        return;
    }

    width = surface->w;
    height = surface->h;

    texture = SDL_CreateTextureFromSurface(SDLManager::getInstance().getRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        Log::getInstance().error("Failed to create texture from surface: " + std::string(SDL_GetError()));
        return;
    }

    Log::getInstance().info("Image loaded successfully. Width: " + std::to_string(width) + ", Height: " + std::to_string(height));
}

void AnimatedSprite::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

void AnimatedSprite::loadFrames(const std::string& imagePath, const std::string& xmlPath) {
    loadTexture(imagePath);
    parseXML(xmlPath);
}

void AnimatedSprite::parseXML(const std::string& xmlPath) {
    Log::getInstance().info("Attempting to parse XML file: " + xmlPath);
    std::ifstream file(xmlPath);
    if (!file.is_open()) {
        Log::getInstance().error("Failed to open XML file: " + xmlPath);
        return;
    }

    std::string line;
    int frameCount = 0;
    while (std::getline(file, line)) {
        if (line.find("<SubTexture") != std::string::npos) {
            Frame frame;
            size_t nameStart = line.find("name=\"") + 6;
            size_t nameEnd = line.find("\"", nameStart);
            frame.name = line.substr(nameStart, nameEnd - nameStart);

            std::istringstream iss(line);
            std::string token;
            while (std::getline(iss, token, ' ')) {
                size_t pos = token.find('=');
                if (pos != std::string::npos) {
                    std::string key = token.substr(0, pos);
                    std::string value = token.substr(pos + 2, token.length() - pos - 3);
                    if (key == "x") frame.x = std::stoi(value);
                    else if (key == "y") frame.y = std::stoi(value);
                    else if (key == "width") frame.width = std::stoi(value);
                    else if (key == "height") frame.height = std::stoi(value);
                    else if (key == "frameX") frame.frameX = std::stoi(value);
                    else if (key == "frameY") frame.frameY = std::stoi(value);
                    else if (key == "frameWidth") frame.frameWidth = std::stoi(value);
                    else if (key == "frameHeight") frame.frameHeight = std::stoi(value);
                }
            }
            frames[frame.name] = frame;
            frameCount++;
        }
    }
}

void AnimatedSprite::addAnimation(const std::string& name, const std::string& prefix, int fps, bool loop) {
    Animation animation;
    animation.name = name;
    animation.frameRate = fps;
    animation.loop = loop;

    for (const auto& pair : frames) {
        if (pair.first.find(prefix) == 0) {
            animation.addFrame(pair.second);
        }
    }

    animations[name] = animation;
}

void AnimatedSprite::addAnimation(const std::string& name, const std::string& prefix, 
                                const std::vector<int>& indices, int fps, bool loop) {
    Animation animation;
    animation.name = name;
    animation.frameRate = fps;
    animation.loop = loop;

    for (int index : indices) {
        std::string frameName = prefix + " " + std::to_string(index);
        if (frames.find(frameName) != frames.end()) {
            animation.addFrame(frames[frameName]);
        }
    }

    animations[name] = animation;
}

void AnimatedSprite::addAnimation(const std::string& name, const std::vector<std::string>& frameNames, int fps, bool loop) {
    Animation animation;
    animation.name = name;
    animation.frameRate = fps;
    animation.loop = loop;

    for (const auto& frameName : frameNames) {
        if (frames.find(frameName) != frames.end()) {
            animation.addFrame(frames[frameName]);
        }
    }

    animations[name] = animation;
}

void AnimatedSprite::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (!currentAnimation || currentAnimation->name != name) {
            currentAnimation = &it->second;
            currentFrame = 0;
            frameTimer = 0;
        }
    } else {
        Log::getInstance().error("Animation not found: " + name);
    }
}

void AnimatedSprite::playAnim(const std::string& name, bool force, AnimationCallback callback) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (force || currentAnimation != &it->second) {
            currentAnimation = &it->second;
            currentFrame = 0;
            frameTimer = 0;
            onAnimationFinished = callback;
        }
    } else {
        Log::getInstance().error("Animation not found: " + name);
    }
}
