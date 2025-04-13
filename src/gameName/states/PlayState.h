#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"
#include "../../engine/Log.h"
#else
#include <State.h>
#include <Sprite.h>
#include <AnimatedSprite.h>
#include <Log.h>
#endif

class PlayState : public State {
public:
    PlayState();
    ~PlayState();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

    void openSubState(SubState* subState);
    static PlayState* instance;

private:
    Sprite* backgroundSprite;
    AnimatedSprite* playerSprite;
};
