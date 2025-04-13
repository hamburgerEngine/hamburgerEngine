#pragma once
#include <SDL2/SDL.h>
#include <map>
#include <functional>
#include <string>
#ifdef __SWITCH__
#include <switch.h>
#endif

class Input {
public:
    static Input& getInstance() {
        static Input instance;
        return instance;
    }

    void handleEvent(const SDL_Event& event);
    void update();
    
    bool isKeyPressed(SDL_Keycode key) const;
    bool isKeyJustPressed(SDL_Keycode key) const;
    bool isKeyJustReleased(SDL_Keycode key) const;
    
    bool isMouseButtonPressed(Uint8 button) const;
    bool isMouseButtonJustPressed(Uint8 button) const;
    bool isMouseButtonJustReleased(Uint8 button) const;
    
    bool isControllerButtonPressed(Uint8 button) const;
    bool isControllerButtonJustPressed(Uint8 button) const;
    bool isControllerButtonJustReleased(Uint8 button) const;
    float getControllerAxis(SDL_GameControllerAxis axis) const;
    bool isControllerConnected() const { 
        #ifdef __SWITCH__
        return true;
        #else
        return sdlController != nullptr; 
        #endif
    }
    
    int getMouseX() const { return mouseX; }
    int getMouseY() const { return mouseY; }
    int getMouseDeltaX() const { return mouseDeltaX; }
    int getMouseDeltaY() const { return mouseDeltaY; }

    void setCallback(const std::string& action, std::function<void()> callback);

private:
    Input();
    ~Input();

    void initController();
    void closeController();
    void handleJoyButtonEvent(const SDL_JoyButtonEvent& event);
    void handleJoyAxisEvent(const SDL_JoyAxisEvent& event);

    std::map<SDL_Keycode, bool> currentKeyState;
    std::map<SDL_Keycode, bool> previousKeyState;
    std::map<SDL_Keycode, bool> justPressedState;
    std::map<SDL_Keycode, bool> justReleasedState;
    
    std::map<Uint8, bool> currentMouseState;
    std::map<Uint8, bool> previousMouseState;
    std::map<Uint8, bool> justPressedMouseState;
    std::map<Uint8, bool> justReleasedMouseState;
    
    #ifdef __SWITCH__
    PadState pad;
    #else
    SDL_GameController* sdlController;
    #endif
    SDL_Joystick* joystick;
    std::map<Uint8, bool> currentControllerState;
    std::map<Uint8, bool> previousControllerState;
    std::map<Uint8, bool> justPressedControllerState;
    std::map<Uint8, bool> justReleasedControllerState;
    std::map<SDL_GameControllerAxis, Sint16> controllerAxisState;
    
    int mouseX, mouseY;
    int mouseDeltaX, mouseDeltaY;
    
    std::map<std::string, std::function<void()>> callbacks;
};
