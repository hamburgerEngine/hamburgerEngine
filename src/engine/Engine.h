#pragma once

#include <vector>
#include <stack>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Text.h"
#include "SoundManager.h"
#include "SDLManager.h"
#include <functional>

class State;
class SubState;

class Engine {
public:
    Engine(int width, int height, const char* title);
    ~Engine();

    void run();
    void update();
    void render();
    void quit();

    static Engine* getInstance() { return instance; }

    void addSprite(Sprite* sprite) { sprites.push_back(sprite); }
    void addAnimatedSprite(AnimatedSprite* sprite) { animatedSprites.push_back(sprite); }
    void addText(Text* text) { texts.push_back(text); }

    void pushState(State* state);
    void popState();
    void switchState(State* state);
    void openSubState(SubState* subState);

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

    void setTimeout(std::function<void()> callback, float seconds);
    void updateTimeouts(float deltaTime);

    float getCurrentTime() const { return SDL_GetTicks() / 1000.0f; }

private:
    static Engine* instance;
    int windowWidth;
    int windowHeight;
    std::vector<Sprite*> sprites;
    std::vector<AnimatedSprite*> animatedSprites;
    std::vector<Text*> texts;
    float deltaTime;
    std::stack<State*> states;
    bool running;

    struct Timeout {
        std::function<void()> callback;
        float remainingTime;
    };
    std::vector<Timeout> timeouts;

    void handleEvents();
};
