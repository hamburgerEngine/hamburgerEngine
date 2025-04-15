#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"
#include "../../engine/Log.h"
#include "../../engine/Paths.h"
#include "../../engine/Button.h"
#else
#include <State.h>
#include <Sprite.h>
#include <AnimatedSprite.h>
#include <Log.h>
#include <Paths.h>
#include <Button.h>
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
