#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Sprite.h"

class AnimatedSprite : public Sprite {
public:
    struct Frame {
        std::string name;
        int x, y, width, height;
        int frameX, frameY, frameWidth, frameHeight;
    };

    struct Animation {
        std::string name;
        std::vector<Frame> frames;
        int frameRate;
        bool loop;

        void addFrame(const Frame& frame) { frames.push_back(frame); }
    };

    AnimatedSprite();
    AnimatedSprite(const std::string& path);
    virtual ~AnimatedSprite();

    virtual void update(float deltaTime) override;
    virtual void render() override;

    void setScale(float scaleX, float scaleY);
    void loadFrames(const std::string& imagePath, const std::string& xmlPath);
    void addAnimation(const std::string& name, const std::string& prefix, int fps, bool loop = true);
    void addAnimation(const std::string& name, const std::string& prefix, const std::vector<int>& indices, int fps, bool loop = true);
    void addAnimation(const std::string& name, const std::vector<std::string>& frameNames, int fps, bool loop = true);
    void playAnimation(const std::string& name);

    bool hasAnimation(const std::string& name) const {
        return animations.find(name) != animations.end();
    }
    
    virtual void playAnim(const std::string& name, bool force = false);
    
    void setOffset(float x, float y) {
        offsetX = x;
        offsetY = y;
    }
    
    void setFlipX(bool flip) {
        scale.x = flip ? -1.0f : 1.0f;
    }

private:
    std::map<std::string, Frame> frames;
    std::map<std::string, Animation> animations;
    Animation* currentAnimation = nullptr;
    int currentFrame = 0;
    float frameTimer = 0;

    void parseXML(const std::string& xmlPath);
    void loadTexture(const std::string& imagePath) override;

protected:
    float offsetX = 0;
    float offsetY = 0;
};
