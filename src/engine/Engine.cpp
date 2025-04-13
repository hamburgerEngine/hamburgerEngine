#ifdef __MINGW32__ || defined(__SWITCH__)
#include "Engine.h"
#include "State.h"
#include "SubState.h"
#include <iostream>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Text.h"
#include "Input.h"
#include <SDL2/SDL_mixer.h>
#else
#include <Engine.h>
#include <State.h>
#include <SubState.h>
#include <iostream>
#include <Sprite.h>
#include <AnimatedSprite.h>
#include <Text.h>
#include <Input.h>
#include <algorithm>
#include <SDL2/SDL_mixer.h>
#endif

Engine* Engine::instance = nullptr;

Engine::Engine(int width, int height, const char* title, int fps)
    : windowWidth(width), windowHeight(height), running(true), fps(fps) {
    if (instance == nullptr) {
        instance = this;
    }

    if (!SDLManager::getInstance().initialize(width, height, title)) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        return;
    }

    Input::initController();

    frameDelay = 1000 / fps;
    debugUI = new DebugUI();
}

Engine::~Engine() {
    Mix_CloseAudio();
    
    for (auto sprite : sprites) {
        delete sprite;
    }
    for (auto animatedSprite : animatedSprites) {
        delete animatedSprite;
    }
    for (auto text : texts) {
        delete text;
    }

    delete debugUI;
}

void Engine::run() {
    const float frameDelay = 1000.0f / fps;
    Uint32 frameStart;
    float frameTime;

    while (running) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Engine::update() {
    static float lastTime = SDL_GetTicks() / 1000.0f;
    float currentTime = SDL_GetTicks() / 1000.0f;
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (!states.empty()) {
        states.top()->update(deltaTime);
    }

    updateTimeouts(deltaTime);
    debugUI->update(deltaTime);
}

void Engine::render() {
    SDLManager::getInstance().clear();

    if (!states.empty()) {
        states.top()->render();
    }

    if (debugMode) {
        debugUI->render();
    }

    SDLManager::getInstance().present();
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit();
                break;
        }
    }
}

void Engine::quit() {
    running = false;
}

void Engine::pushState(State* state) {
    states.push(state);
    state->create();
}

void Engine::popState() {
    if (!states.empty()) {
        states.top()->destroy();
        delete states.top();
        states.pop();
    }
}

void Engine::switchState(State* state) {
    if (!states.empty()) {
        State* oldState = states.top();
        states.pop();
        oldState->destroy();
        delete oldState;
        
        sprites.clear();
        animatedSprites.clear();
        texts.clear();
    }
    pushState(state);
}

void Engine::openSubState(SubState* subState) {
    std::cout << "Engine::openSubState called" << std::endl;
    if (!states.empty()) {
        states.top()->openSubState(subState);
    } else {
        std::cout << "No states to open substate on" << std::endl;
    }
}

void Engine::setTimeout(std::function<void()> callback, float seconds) {
    Timeout timeout;
    timeout.callback = callback;
    timeout.remainingTime = seconds;
    timeouts.push_back(timeout);
}

void Engine::updateTimeouts(float deltaTime) {
    for (auto it = timeouts.begin(); it != timeouts.end();) {
        it->remainingTime -= deltaTime;
        if (it->remainingTime <= 0) {
            it->callback();
            it = timeouts.erase(it);
        } else {
            ++it;
        }
    }
}
