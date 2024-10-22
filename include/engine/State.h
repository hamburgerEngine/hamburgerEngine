#pragma once
#include "Engine.h"

class State {
public:
    virtual ~State() {}
    virtual void create() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};
