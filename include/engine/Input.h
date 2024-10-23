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

    void update();
    void keyPressed(unsigned char key);
    void keyReleased(unsigned char key);
    bool isKeyPressed(unsigned char key);
    bool isKeyJustPressed(unsigned char key);
    bool isKeyJustReleased(unsigned char key);

    void setCallback(const std::string& action, std::function<void()> callback);

    std::string getActionForKey(unsigned char key);

private:
    Input();
    std::array<bool, 128> currentKeyState;
    std::array<bool, 128> previousKeyState;
    std::array<bool, 128> justPressedState;
    std::array<bool, 128> justReleasedState;
    std::map<std::string, std::function<void()>> callbacks;

    void handleKeyPress(unsigned char key);
    void handleKeyRelease(unsigned char key);
    bool isValidKey(unsigned char key);
};
