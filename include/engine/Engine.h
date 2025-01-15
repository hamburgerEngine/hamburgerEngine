#pragma once

#include <GL/glut.h>
#include <vector>
#include <stack>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Text.h"
#include "SoundManager.h"

class State;
class SubState;

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

    void pushState(State* state);
    void popState();
    void switchState(State* state);
    void openSubState(SubState* subState);

    static void keyboardCallback(unsigned char key, int x, int y);
    static void keyboardUpCallback(unsigned char key, int x, int y);

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }

    State* getCurrentState() { return states.empty() ? nullptr : states.top(); }

    void clearSprites() { sprites.clear(); }
    void clearAnimatedSprites() { animatedSprites.clear(); }
    void clearTexts() { texts.clear(); }
    
    void clearAllSprites() {
        sprites.clear();
        animatedSprites.clear();
        texts.clear();
    }

    SoundManager& getSoundManager() { return SoundManager::getInstance(); }

    const std::vector<Sprite*>& getSprites() const { return sprites; }
    const std::vector<AnimatedSprite*>& getAnimatedSprites() const { return animatedSprites; }

private:
    static Engine* instance;
    int windowWidth;
    int windowHeight;
    std::vector<Sprite*> sprites;
    std::vector<AnimatedSprite*> animatedSprites;
    std::vector<Text*> texts;
    float deltaTime;
    std::stack<State*> states;

    static void displayCallback();
    static void idleCallback();
    static void reshapeCallback(int width, int height);
    static void keyPressedCallback(unsigned char key, int x, int y);
    static void keyReleasedCallback(unsigned char key, int x, int y);
    static void specialKeyCallback(int key, int x, int y);
    static void specialKeyUpCallback(int key, int x, int y);
};
