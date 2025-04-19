#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../../engine/core/State.h"
#include "../../engine/graphics/Sprite.h"
#include "../../engine/graphics/AnimatedSprite.h"
#include "../../engine/utils/Log.h"
#include "../../engine/utils/Paths.h"
#include "../../engine/graphics/Button.h"
#else
#include <core/State.h>
#include <graphics/Sprite.h>
#include <graphics/AnimatedSprite.h>
#include <utils/Log.h>
#include <utils/Paths.h>
#include <graphics/Button.h>
#endif

class BaseState : public State {
public:
    static BaseState* instance;
    
    BaseState();
    ~BaseState() override;
    
    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;
    
    void openSubState(SubState* subState) override;

private:
};
