#pragma once
#include <string>

class Sprite {
public:
    struct Scale {
        float x;
        float y;
        Scale() : x(1.0f), y(1.0f) {}
        void set(float newX, float newY) { x = newX; y = newY; }
    };

    Sprite(const std::string& imagePath);
    virtual ~Sprite();

    virtual void update();
    virtual void render();

    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    Scale scale;

private:
    float x, y;
    int width, height;
    unsigned char* imageData;
    unsigned int textureID;

    void loadTexture(const std::string& imagePath);
};
