#pragma once

#ifdef __MINGW32__
#include "SDLManager.h"
#elif defined(__SWITCH__)
#include "SDLManager.h"
#include <switch.h>
#else
#include <SDLManager.h>
#endif
#include <unordered_set>
#include <unordered_map>

class Input {
public:
    static void UpdateKeyStates();
    static bool justPressed(SDL_Scancode key);
    static bool justReleased(SDL_Scancode key);
    static bool pressed(SDL_Scancode key);
    
    static void initController();
    static void closeController();
    static void UpdateControllerStates();
    static bool isControllerButtonJustPressed(Uint8 button);
    static bool isControllerButtonJustReleased(Uint8 button);
    static bool isControllerButtonPressed(Uint8 button);
    static Sint16 getControllerAxis(SDL_GameControllerAxis axis);
    static void handleJoyButtonEvent(const SDL_JoyButtonEvent& event);

private:
    static std::unordered_set<SDL_Scancode> currentPressedKeys;
    static std::unordered_set<SDL_Scancode> previousPressedKeys;
    
    static std::unordered_map<Uint8, bool> currentControllerState;
    static std::unordered_map<Uint8, bool> previousControllerState;
    static std::unordered_map<SDL_GameControllerAxis, Sint16> controllerAxisState;
    
    #ifdef __SWITCH__
    static PadState pad;
    #else
    static SDL_GameController* sdlController;
    #endif
};