#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Log.h"

class SDLManager {
public:
    static SDLManager& getInstance() {
        static SDLManager instance;
        return instance;
    }

    bool initialize(int width, int height, const char* title);
    void shutdown();
    
    SDL_Window* getWindow() const { return window; }
    SDL_Renderer* getRenderer() const { return renderer; }
    
    void clear();
    void present();
    
    void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void resetColor();

private:
    SDLManager() : window(nullptr), renderer(nullptr) {}
    ~SDLManager();
    SDLManager(const SDLManager&) = delete;
    SDLManager& operator=(const SDLManager&) = delete;

    SDL_Window* window;
    SDL_Renderer* renderer;
}; 