#pragma once
#include "State.h"

class SubState : public State {
public:
    SubState() : _parentState(nullptr) {}
    virtual ~SubState() {}

    void setParentState(State* parentState) { _parentState = parentState; }
    State* getParentState() const { return _parentState; }

    virtual void create() override = 0;
    virtual void update(float deltaTime) override = 0;
    virtual void render() override = 0;
    virtual void destroy() override = 0;

private:
    State* _parentState;
};
