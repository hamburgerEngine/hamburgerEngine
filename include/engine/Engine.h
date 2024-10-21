#pragma once

#include <GL/glut.h>
#include <vector>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Text.h"

class Engine {
public:
    Engine(int width, int height, const char* title);
    ~Engine();

    void run();
    void update();
    void render();

    static Engine* getInstance() { return instance; }

    void addSprite(Sprite* sprite) { sprites.push_back(sprite); }
    void addAnimatedSprite(AnimatedSprite* sprite) { animatedSprites.push_back(sprite); }
    void addText(Text* text) { texts.push_back(text); }

private:
    static Engine* instance;
    int windowWidth;
    int windowHeight;
    std::vector<Sprite*> sprites;
    std::vector<AnimatedSprite*> animatedSprites;
    std::vector<Text*> texts;
    float deltaTime;

    static void displayCallback();
    static void idleCallback();
    static void reshapeCallback(int width, int height);
};
