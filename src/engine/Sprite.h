#pragma once
#include <string>
#include <SDL2/SDL.h>

class Camera;

class Sprite {
protected:
    bool visible = true;
    std::string imagePath;
    float x = 0, y = 0;
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    Camera* camera = nullptr;  

public:
    struct Scale {
        float x;
        float y;
        Scale() : x(1.0f), y(1.0f) {}
        void set(float newX, float newY) { x = newX; y = newY; }
    };

    Sprite();
    Sprite(const std::string& path);
    virtual ~Sprite(); 

    virtual void update(float deltaTime) {}
    virtual void render(); 

    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    Scale scale;

    void setScale(float scaleX, float scaleY);

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    const Scale& getScale() const { return scale; }

    void setVisible(bool visible) { this->visible = visible; }
    bool isVisible() const { return visible; }

    void setCamera(Camera* cam) { camera = cam; }
    Camera* getCamera() const { return camera; }

protected:
    virtual void loadTexture(const std::string& imagePath);
};
