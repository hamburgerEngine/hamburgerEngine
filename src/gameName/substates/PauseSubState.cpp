#ifdef __MINGW32__ || defined(__SWITCH__)
#include "gameName/substates/PauseSubState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "engine/SDLManager.h"
#include <iostream>
#else
#include <substates/PauseSubState.h>
#include <Engine.h>
#include <Input.h>
#include <SDLManager.h>
#include <iostream>
#endif

PauseSubState::PauseSubState() : pauseText(nullptr) {
}

PauseSubState::~PauseSubState() {
    destroy();
}

void PauseSubState::create() {
    pauseText = new Text(300, 250, 200);
    pauseText->setText("PAUSED");
    pauseText->setFormat("assets/fonts/Zero G.ttf", 36, 0xFFFFFFFF);
}

void PauseSubState::update(float deltaTime) {
    if (Input::justPressed(SDL_SCANCODE_RETURN) || Input::isControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_START)) {
        std::cout << "Start button pressed in PauseSubState, closing" << std::endl;
        getParentState()->closeSubState();
    }
}

void PauseSubState::render() {
    SDL_SetRenderDrawColor(SDLManager::getInstance().getRenderer(), 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(SDLManager::getInstance().getRenderer(), &overlay);
    SDL_SetRenderDrawColor(SDLManager::getInstance().getRenderer(), 255, 255, 255, 255);

    pauseText->render();
}

void PauseSubState::destroy() {
    if (pauseText) {
        delete pauseText;
        pauseText = nullptr;
    }
}
