#ifdef __MINGW32__
#include "Input.h"
#elif defined(__SWITCH__)
#include "Input.h"
#include <switch.h>
#else
#include <input/Input.h>
#endif
#include "../utils/Log.h"

std::unordered_set<SDL_Scancode> Input::currentPressedKeys;
std::unordered_set<SDL_Scancode> Input::previousPressedKeys;

std::unordered_map<Uint8, bool> Input::currentControllerState;
std::unordered_map<Uint8, bool> Input::previousControllerState;
std::unordered_map<SDL_GameControllerAxis, Sint16> Input::controllerAxisState;

#ifdef __SWITCH__
PadState Input::pad;
#else
SDL_GameController* Input::sdlController = nullptr;
#endif

void Input::initController() {
    #ifdef __SWITCH__
    Log::getInstance().info("Initializing Switch controller");
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);
    Log::getInstance().info("Switch controller initialized");
    #else
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
        Log::getInstance().error("Failed to initialize game controller subsystem: " + std::string(SDL_GetError()));
        return;
    }

    Log::getInstance().info("Loading game controller mappings from assets/data/gamecontrollerdb.txt");
    int mappingsLoaded = SDL_GameControllerAddMappingsFromFile("assets/data/gamecontrollerdb.txt");
    Log::getInstance().info("Loaded " + std::to_string(mappingsLoaded) + " controller mappings");

    int numJoysticks = SDL_NumJoysticks();
    Log::getInstance().info("Found " + std::to_string(numJoysticks) + " joystick devices");

    for (int i = 0; i < numJoysticks; i++) {
        if (SDL_IsGameController(i)) {
            Log::getInstance().info("Joystick " + std::to_string(i) + " is a game controller: " + std::string(SDL_GameControllerNameForIndex(i)));
            sdlController = SDL_GameControllerOpen(i);
            if (sdlController) {
                Log::getInstance().info("Successfully opened controller: " + std::string(SDL_GameControllerName(sdlController)));
                break;
            } else {
                Log::getInstance().error("Failed to open controller: " + std::string(SDL_GetError()));
            }
        } else {
            Log::getInstance().info("Joystick " + std::to_string(i) + " is not a game controller");
        }
    }
    #endif
}

void Input::closeController() {
    #ifdef __SWITCH__
    // nan
    #else
    if (sdlController) {
        SDL_GameControllerClose(sdlController);
        sdlController = nullptr;
    }
    #endif
}

void Input::UpdateKeyStates() {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    previousPressedKeys = currentPressedKeys;
    currentPressedKeys.clear();

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        if (state[i]) {
            currentPressedKeys.insert(static_cast<SDL_Scancode>(i));
        }
    }
}

bool Input::justPressed(SDL_Scancode key) {
    return currentPressedKeys.find(key) != currentPressedKeys.end() &&
        previousPressedKeys.find(key) == previousPressedKeys.end();
}

bool Input::justReleased(SDL_Scancode key) {
    return previousPressedKeys.find(key) != previousPressedKeys.end() &&
        currentPressedKeys.find(key) == currentPressedKeys.end();
}

bool Input::pressed(SDL_Scancode key) {
    return currentPressedKeys.find(key) != currentPressedKeys.end();
}

void Input::UpdateControllerStates() {
    #ifdef __SWITCH__
    padUpdate(&pad);
    HidAnalogStickState analog_stick_l = padGetStickPos(&pad, 0);
    HidAnalogStickState analog_stick_r = padGetStickPos(&pad, 1);
    
    controllerAxisState[SDL_CONTROLLER_AXIS_LEFTX] = analog_stick_l.x;
    controllerAxisState[SDL_CONTROLLER_AXIS_LEFTY] = analog_stick_l.y;
    controllerAxisState[SDL_CONTROLLER_AXIS_RIGHTX] = analog_stick_r.x;
    controllerAxisState[SDL_CONTROLLER_AXIS_RIGHTY] = analog_stick_r.y;
    
    u64 buttons = padGetButtons(&pad);
    
    previousControllerState = currentControllerState;
    
    currentControllerState[SDL_CONTROLLER_BUTTON_A] = (buttons & HidNpadButton_A) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_B] = (buttons & HidNpadButton_B) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_X] = (buttons & HidNpadButton_X) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_Y] = (buttons & HidNpadButton_Y) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = (buttons & HidNpadButton_L) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = (buttons & HidNpadButton_R) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_START] = (buttons & HidNpadButton_Plus) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_BACK] = (buttons & HidNpadButton_Minus) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_DPAD_UP] = (buttons & HidNpadButton_Up) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = (buttons & HidNpadButton_Down) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = (buttons & HidNpadButton_Left) != 0;
    currentControllerState[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = (buttons & HidNpadButton_Right) != 0;
    #else
    if (sdlController) {
        previousControllerState = currentControllerState;
        
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
            currentControllerState[i] = SDL_GameControllerGetButton(sdlController, static_cast<SDL_GameControllerButton>(i));
        }
        
        for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++) {
            controllerAxisState[static_cast<SDL_GameControllerAxis>(i)] = 
                SDL_GameControllerGetAxis(sdlController, static_cast<SDL_GameControllerAxis>(i));
        }
    } else {
        static bool logged = false;
        if (!logged) {
            Log::getInstance().warning("No controller connected or initialized");
            logged = true;
        }
    }
    #endif
}

void Input::handleJoyButtonEvent(const SDL_JoyButtonEvent& event) {
    #ifdef __SWITCH__
    Uint8 mappedButton = event.button;
    switch (event.button) {
        case 0:
            mappedButton = SDL_CONTROLLER_BUTTON_A;
            break;
        case 1:
            mappedButton = SDL_CONTROLLER_BUTTON_B;
            break;
        case 2:
            mappedButton = SDL_CONTROLLER_BUTTON_X;
            break;
        case 3:
            mappedButton = SDL_CONTROLLER_BUTTON_Y;
            break;
        case 4:
            mappedButton = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
            break;
        case 5:
            mappedButton = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
            break;
        case 6:
            mappedButton = SDL_CONTROLLER_BUTTON_START;
            break;
        case 7:
            mappedButton = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
            break;
        case 8:
            mappedButton = SDL_CONTROLLER_BUTTON_BACK;
            break;
        case 9:
            mappedButton = SDL_CONTROLLER_BUTTON_LEFTSTICK;
            break;
        case 10:
            mappedButton = SDL_CONTROLLER_BUTTON_LEFTSTICK;
            break;
        case 11:
            mappedButton = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
            break;
        case 12:
            mappedButton = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
            break;
        case 13:
            mappedButton = SDL_CONTROLLER_BUTTON_DPAD_UP;
            break;
        case 14:
            mappedButton = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
            break;
        case 15:
            mappedButton = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
            break;
    }

    if (event.state == SDL_PRESSED) {
        currentControllerState[mappedButton] = true;
    } else {
        currentControllerState[mappedButton] = false;
    }
    #endif
}

bool Input::isControllerButtonJustPressed(Uint8 button) {
    return currentControllerState[button] && !previousControllerState[button];
}

bool Input::isControllerButtonJustReleased(Uint8 button) {
    return !currentControllerState[button] && previousControllerState[button];
}

bool Input::isControllerButtonPressed(Uint8 button) {
    return currentControllerState[button];
}

Sint16 Input::getControllerAxis(SDL_GameControllerAxis axis) {
    return controllerAxisState[axis];
}