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
    void specialKeyPressed(int key);
    void specialKeyReleased(int key);
    bool isKeyPressed(int key);
    bool isKeyJustPressed(int key);
    bool isKeyJustReleased(int key);
    void update();

    static void handleKeyPress(int key) {
        getInstance().specialKeyPressed(key);
    }
    
    static void handleKeyRelease(int key) {
        getInstance().specialKeyReleased(key);
    }
    
    static bool justPressed(int key) {
        return getInstance().isKeyJustPressed(key);
    }
    
    static bool pressed(int key) {
        return getInstance().isKeyPressed(key);
    }
    
    static bool justReleased(int key) {
        return getInstance().isKeyJustReleased(key);
    }

    void setCallback(const std::string& action, std::function<void()> callback);

private:
    Input();
    std::array<bool, 256> currentKeyState;
    std::array<bool, 256> previousKeyState;
    std::array<bool, 256> justPressedState;
    std::array<bool, 256> justReleasedState;
    std::map<std::string, std::function<void()>> callbacks;

    bool isValidKey(int key) const {
        return key >= 0 && key < 256;
    }
};
