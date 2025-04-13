#include "Camera.h"
#include "SDLManager.h"

Camera::Camera() {}

void Camera::begin() {
    if (!visible) return;
    
    SDL_Renderer* renderer = SDLManager::getInstance().getRenderer();
    SDL_RenderSetScale(renderer, zoom, zoom);
    SDL_RenderSetViewport(renderer, nullptr);
    SDL_RenderSetClipRect(renderer, nullptr);
}

void Camera::end() {
    if (!visible) return;
    SDL_Renderer* renderer = SDLManager::getInstance().getRenderer();
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}
