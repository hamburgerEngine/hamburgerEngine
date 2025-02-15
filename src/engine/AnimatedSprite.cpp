#include <AnimatedSprite.h>
#include <GL/glut.h>
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

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
            }
        }
        frameTimer -= frameDuration;
    }
}

void AnimatedSprite::render() {
    if (!visible || !currentAnimation || currentAnimation->frames.empty()) {
        if (!visible) std::cout << "Not visible" << std::endl;
        if (!currentAnimation) std::cout << "No current animation" << std::endl;
        if (currentAnimation && currentAnimation->frames.empty()) std::cout << "No frames in animation" << std::endl;
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    glTranslatef(x + offsetX, y + offsetY, 0);
    glScalef(scale.x, scale.y, 1.0f);

    const Frame& frame = currentAnimation->frames[currentFrame];
    
    float texLeft = (float)frame.x / width;
    float texRight = (float)(frame.x + frame.width) / width;
    float texTop = (float)frame.y / height;
    float texBottom = (float)(frame.y + frame.height) / height;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);    glVertex2f(0, 0);
    glTexCoord2f(texRight, texTop);   glVertex2f(frame.width, 0);
    glTexCoord2f(texRight, texBottom); glVertex2f(frame.width, frame.height);
    glTexCoord2f(texLeft, texBottom);  glVertex2f(0, frame.height);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND); 
}

void AnimatedSprite::loadTexture(const std::string& imagePath) {
    if (imageData) {
        std::cout << "Texture already loaded, skipping." << std::endl;
        return;
    }

    std::cout << "Attempting to load image from: " << imagePath << std::endl;
    
    int channels;
    imageData = stbi_load(imagePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!imageData) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        std::cerr << "STB Image error: " << stbi_failure_reason() << std::endl;
        return;
    }

    std::cout << "Image loaded successfully. Width: " << width << ", Height: " << height << ", Channels: " << channels << std::endl;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
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
    std::cout << "Attempting to parse XML file: " << xmlPath << std::endl;
    std::ifstream file(xmlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open XML file: " << xmlPath << std::endl;
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
            std::cout << "Loaded frame: " << frame.name << std::endl;
        }
    }
    std::cout << "Total frames loaded: " << frameCount << std::endl;
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

    std::cout << "Added animation '" << name << "' with " << animation.frames.size() << " frames" << std::endl;
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
            std::cout << "Playing animation: " << name << " with " << currentAnimation->frames.size() << " frames" << std::endl;
        }
    } else {
        std::cerr << "Animation not found: " << name << std::endl;
    }
}

void AnimatedSprite::playAnim(const std::string& name, bool force) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (force || currentAnimation != &it->second) {
            currentAnimation = &it->second;
            currentFrame = 0;
            frameTimer = 0;
        }
    } else {
        std::cerr << "Animation not found: " << name << std::endl;
    }
}
