#ifdef __MINGW32__
#include "Input.h"
#elif defined(__SWITCH__)
#include "Input.h"
#include <switch.h>
#else
#include <Input.h>
#endif
#include "Log.h"
#include <iostream>

Input::Input() : mouseX(0), mouseY(0), mouseDeltaX(0), mouseDeltaY(0)
#ifdef __SWITCH__
    , pad()
#else
    , sdlController(nullptr)
#endif
    , joystick(nullptr)
{
    initController();
}

Input::~Input() {
    closeController();
}

void Input::initController() {
    #ifdef __SWITCH__
    Log::getInstance().info("Initializing Switch controller");
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);
    Log::getInstance().info("Switch controller initialized");

    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        Log::getInstance().error("Failed to initialize joystick subsystem: " + std::string(SDL_GetError()));
        return;
    }

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        joystick = SDL_JoystickOpen(i);
        if (joystick) {
            Log::getInstance().info("Joystick connected: " + std::string(SDL_JoystickName(joystick)));
            break;
        }
    }
    #else
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
        Log::getInstance().error("Failed to initialize game controller subsystem: " + std::string(SDL_GetError()));
        return;
    }

    SDL_GameControllerAddMappingsFromFile("assets/data/gamecontrollerdb.txt");

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            sdlController = SDL_GameControllerOpen(i);
            if (sdlController) {
                Log::getInstance().info("Controller connected: " + std::string(SDL_GameControllerName(sdlController)));
                break;
            }
        }
    }
    #endif
}

void Input::closeController() {
    #ifdef __SWITCH__
    if (joystick) {
        SDL_JoystickClose(joystick);
        joystick = nullptr;
    }
    #else
    if (sdlController) {
        SDL_GameControllerClose(sdlController);
        sdlController = nullptr;
    }
    #endif
}

void Input::handleJoyButtonEvent(const SDL_JoyButtonEvent& event) {
    if (event.which != SDL_JoystickInstanceID(joystick)) return;

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
        justPressedControllerState[mappedButton] = true;
        Log::getInstance().debug("Joystick button pressed: " + std::to_string(event.button) + " mapped to: " + std::to_string(mappedButton));
    } else {
        currentControllerState[mappedButton] = false;
        justReleasedControllerState[mappedButton] = true;
        Log::getInstance().debug("Joystick button released: " + std::to_string(event.button) + " mapped to: " + std::to_string(mappedButton));
    }
}

void Input::handleJoyAxisEvent(const SDL_JoyAxisEvent& event) {
    if (event.which != SDL_JoystickInstanceID(joystick)) return;

    switch (event.axis) {
        case 0:
            controllerAxisState[SDL_CONTROLLER_AXIS_LEFTX] = event.value;
            break;
        case 1:
            controllerAxisState[SDL_CONTROLLER_AXIS_LEFTY] = event.value;
            break;
        case 2:
            controllerAxisState[SDL_CONTROLLER_AXIS_RIGHTX] = event.value;
            break;
        case 3:
            controllerAxisState[SDL_CONTROLLER_AXIS_RIGHTY] = event.value;
            break;
    }
}

void Input::handleEvent(const SDL_Event& event) {
    #ifdef __SWITCH__
    padUpdate(&pad);
    HidAnalogStickState analog_stick_l = padGetStickPos(&pad, 0);
    HidAnalogStickState analog_stick_r = padGetStickPos(&pad, 1);
    
    controllerAxisState[SDL_CONTROLLER_AXIS_LEFTX] = analog_stick_l.x;
    controllerAxisState[SDL_CONTROLLER_AXIS_LEFTY] = analog_stick_l.y;
    controllerAxisState[SDL_CONTROLLER_AXIS_RIGHTX] = analog_stick_r.x;
    controllerAxisState[SDL_CONTROLLER_AXIS_RIGHTY] = analog_stick_r.y;
    
    u64 buttons = padGetButtons(&pad);
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

    switch (event.type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            handleJoyButtonEvent(event.jbutton);
            break;
        case SDL_JOYAXISMOTION:
            handleJoyAxisEvent(event.jaxis);
            break;
    }
    #else
    switch (event.type) {
        case SDL_KEYDOWN:
            if (!event.key.repeat) {
                currentKeyState[event.key.keysym.sym] = true;
                justPressedState[event.key.keysym.sym] = true;
            }
            break;
            
        case SDL_KEYUP:
            currentKeyState[event.key.keysym.sym] = false;
            justReleasedState[event.key.keysym.sym] = true;
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            currentMouseState[event.button.button] = true;
            justPressedMouseState[event.button.button] = true;
            break;
            
        case SDL_MOUSEBUTTONUP:
            currentMouseState[event.button.button] = false;
            justReleasedMouseState[event.button.button] = true;
            break;
            
        case SDL_MOUSEMOTION:
            mouseDeltaX = event.motion.xrel;
            mouseDeltaY = event.motion.yrel;
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            if (event.cbutton.which == SDL_GameControllerGetPlayerIndex(sdlController)) {
                currentControllerState[event.cbutton.button] = true;
                justPressedControllerState[event.cbutton.button] = true;
                Log::getInstance().debug("Controller button pressed: " + std::to_string(event.cbutton.button) + " mapped to: " + std::to_string(event.cbutton.button));
            }
            break;

        case SDL_CONTROLLERBUTTONUP:
            if (event.cbutton.which == SDL_GameControllerGetPlayerIndex(sdlController)) {
                currentControllerState[event.cbutton.button] = false;
                justReleasedControllerState[event.cbutton.button] = true;
                Log::getInstance().debug("Controller button released: " + std::to_string(event.cbutton.button) + " mapped to: " + std::to_string(event.cbutton.button));
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            if (event.caxis.which == SDL_GameControllerGetPlayerIndex(sdlController)) {
                controllerAxisState[static_cast<SDL_GameControllerAxis>(event.caxis.axis)] = event.caxis.value;
            }
            break;

        case SDL_CONTROLLERDEVICEADDED:
            if (!sdlController) {
                sdlController = SDL_GameControllerOpen(event.cdevice.which);
                if (sdlController) {
                    Log::getInstance().info("Controller connected: " + std::string(SDL_GameControllerName(sdlController)));
                }
            }
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            if (sdlController && event.cdevice.which == SDL_GameControllerGetPlayerIndex(sdlController)) {
                Log::getInstance().info("Controller disconnected");
                closeController();
            }
            break;
    }
    #endif
}

void Input::update() {
    for (auto& pair : currentKeyState) {
        previousKeyState[pair.first] = pair.second;
    }
    justPressedState.clear();
    justReleasedState.clear();
    
    for (auto& pair : currentMouseState) {
        previousMouseState[pair.first] = pair.second;
    }
    justPressedMouseState.clear();
    justReleasedMouseState.clear();
    
    for (auto& pair : currentControllerState) {
        previousControllerState[pair.first] = pair.second;
    }
    justPressedControllerState.clear();
    justReleasedControllerState.clear();
    
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}

bool Input::isKeyPressed(SDL_Keycode key) const {
    auto it = currentKeyState.find(key);
    return it != currentKeyState.end() && it->second;
}

bool Input::isKeyJustPressed(SDL_Keycode key) const {
    auto it = justPressedState.find(key);
    return it != justPressedState.end() && it->second;
}

bool Input::isKeyJustReleased(SDL_Keycode key) const {
    auto it = justReleasedState.find(key);
    return it != justReleasedState.end() && it->second;
}

bool Input::isMouseButtonPressed(Uint8 button) const {
    auto it = currentMouseState.find(button);
    return it != currentMouseState.end() && it->second;
}

bool Input::isMouseButtonJustPressed(Uint8 button) const {
    auto it = justPressedMouseState.find(button);
    return it != justPressedMouseState.end() && it->second;
}

bool Input::isMouseButtonJustReleased(Uint8 button) const {
    auto it = justReleasedMouseState.find(button);
    return it != justReleasedMouseState.end() && it->second;
}

bool Input::isControllerButtonPressed(Uint8 button) const {
    #ifdef __SWITCH__
    auto it = currentControllerState.find(button);
    return it != currentControllerState.end() && it->second;
    #else
    if (!sdlController) return false;
    auto it = currentControllerState.find(button);
    return it != currentControllerState.end() && it->second;
    #endif
}

bool Input::isControllerButtonJustPressed(Uint8 button) const {
    #ifdef __SWITCH__
    auto it = justPressedControllerState.find(button);
    return it != justPressedControllerState.end() && it->second;
    #else
    if (!sdlController) return false;
    auto it = justPressedControllerState.find(button);
    return it != justPressedControllerState.end() && it->second;
    #endif
}

bool Input::isControllerButtonJustReleased(Uint8 button) const {
    #ifdef __SWITCH__
    auto it = justReleasedControllerState.find(button);
    return it != justReleasedControllerState.end() && it->second;
    #else
    if (!sdlController) return false;
    auto it = justReleasedControllerState.find(button);
    return it != justReleasedControllerState.end() && it->second;
    #endif
}

float Input::getControllerAxis(SDL_GameControllerAxis axis) const {
    #ifdef __SWITCH__
    auto it = controllerAxisState.find(axis);
    if (it == controllerAxisState.end()) return 0.0f;
    
    float value = it->second / 32767.0f;
    if (std::abs(value) < 0.1f) value = 0.0f;
    return value;
    #else
    if (!sdlController) return 0.0f;
    auto it = controllerAxisState.find(axis);
    if (it == controllerAxisState.end()) return 0.0f;
    
    float value = it->second / 32767.0f;
    if (std::abs(value) < 0.1f) value = 0.0f;
    return value;
    #endif
}

void Input::setCallback(const std::string& action, std::function<void()> callback) {
    callbacks[action] = callback;
}
