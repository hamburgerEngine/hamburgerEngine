#pragma once

#include "../graphics/Text.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif defined(__SWITCH__)
#include <switch.h>
#endif

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
    
    void updateMemoryStatsWindows();
    void updateMemoryStatsSwitch();
}; 