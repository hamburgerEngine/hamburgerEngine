#pragma once
#include <vector>

class SubState;

class State {
public:
    virtual ~State() {}
    virtual void create() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;

    virtual void openSubState(SubState* subState);
    virtual void closeSubState();
    virtual void updateSubState(float deltaTime);
    virtual void renderSubState();

protected:
    std::vector<SubState*> _subStates;
};
