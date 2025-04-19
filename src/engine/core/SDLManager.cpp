#include <SDLManager.h>
#include <iostream>

bool SDLManager::initialize(int width, int height, const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                            width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return true;
}

void SDLManager::shutdown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDLManager::~SDLManager() {
    shutdown();
}

void SDLManager::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void SDLManager::present() {
    SDL_RenderPresent(renderer);
}

void SDLManager::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void SDLManager::resetColor() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
} 