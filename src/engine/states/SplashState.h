#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../core/State.h"
#include "../graphics/Sprite.h"
#include "../graphics/AnimatedSprite.h"
#include "../utils/Log.h"
#include "../utils/Paths.h"
#include "../graphics/Button.h"
#include "../core/SDLManager.h"
#else
#include <core/State.h>
#include <graphics/Sprite.h>
#include <graphics/AnimatedSprite.h>
#include <utils/Log.h>
#include <utils/Paths.h>
#include <graphics/Button.h>
#include <core/SDLManager.h>
#endif

class SplashState : public State {
public:
    static SplashState* instance;
    
    SplashState();
    ~SplashState() override;
    
    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;
    
    void openSubState(SubState* subState) override;

    void fadeIn(float duration = 1.0f);
    void fadeOut(float duration = 1.0f);
    bool isFading() const { return fadeInProgress || fadeOutProgress; }
    float getAlpha() const { return alpha; }

private:
    float alpha = 0.0f;
    float fadeInProgress = 0.0f;
    float fadeOutProgress = 0.0f;
    float fadeInDuration = 1.0f;
    float fadeOutDuration = 1.0f;
    
    Sprite* logoSprite = nullptr;
};
