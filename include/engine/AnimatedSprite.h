#pragma once
#include <string>
#include <map>
#include <vector>

class AnimatedSprite {
public:
    struct Scale {
        float x;
        float y;
        Scale() : x(1.0f), y(1.0f) {}
        void set(float newX, float newY) { x = newX; y = newY; }
    };

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

    AnimatedSprite(const std::string& imagePath = "");
    virtual ~AnimatedSprite();

    virtual void update(float deltaTime);
    virtual void render();

    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    Scale scale;

    void setScale(float scaleX, float scaleY);
    void loadFrames(const std::string& imagePath, const std::string& xmlPath);
    void addAnimation(const std::string& name, const std::string& prefix, 
                     int fps, bool loop = true);
    void addAnimation(const std::string& name, const std::string& prefix, 
                     const std::vector<int>& indices, int fps, bool loop = true);
    void playAnimation(const std::string& name);

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void addAnimation(const std::string& name, const std::vector<std::string>& frameNames, int fps, bool loop = true);

private:
    float x, y;
    int width, height;
    unsigned char* imageData;
    unsigned int textureID;
    std::map<std::string, Frame> frames;
    std::map<std::string, Animation> animations;
    Animation* currentAnimation;
    int currentFrame;
    float frameTimer;

    void loadTexture(const std::string& imagePath);
    void parseXML(const std::string& xmlPath);
};
