#pragma once

#include "Text.h"
#include <string>

class DebugUI {
public:
    DebugUI();
    ~DebugUI();

    void update(float deltaTime);
    void render();

private:
    Text* fpsText;
    Text* ramText;
    Text* memoryText;
    
    float fpsUpdateTimer;
    static constexpr float FPS_UPDATE_INTERVAL = 0.5f;
    float currentFPS;
    
    void updateFPS(float deltaTime);
    void updateMemoryStats();
}; 