#pragma once
#include <map>
#include <functional>
#include <array>
#include <string>

class Input {
public:
    static Input& getInstance() {
        static Input instance;
        return instance;
    }

    void keyPressed(unsigned char key);
    void keyReleased(unsigned char key);
    bool isKeyPressed(unsigned char key);
    bool isKeyJustPressed(unsigned char key);
    bool isKeyJustReleased(unsigned char key);
    void update();

    static void handleKeyPress(int key) {
        getInstance().keyPressed(key);
    }
    
    static void handleKeyRelease(int key) {
        getInstance().keyReleased(static_cast<unsigned char>(key));
    }
    
    static bool justPressed(int key) {
        return getInstance().isKeyJustPressed(static_cast<unsigned char>(key));
    }
    
    static bool pressed(int key) {
        return getInstance().isKeyPressed(static_cast<unsigned char>(key));
    }
    
    static bool justReleased(int key) {
        return getInstance().isKeyJustReleased(static_cast<unsigned char>(key));
    }

    void setCallback(const std::string& action, std::function<void()> callback);

private:
    Input();
    std::array<bool, 256> currentKeyState;
    std::array<bool, 256> previousKeyState;
    std::array<bool, 256> justPressedState;
    std::array<bool, 256> justReleasedState;
    std::map<std::string, std::function<void()>> callbacks;

    bool isValidKey(unsigned char key);
};
