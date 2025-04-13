#pragma once

#ifdef __MINGW32__
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"
#elif defined(__SWITCH__)
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"
#else
#include <State.h>
#include <Sprite.h>
#include <AnimatedSprite.h>
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
