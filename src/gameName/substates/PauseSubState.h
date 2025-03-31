#pragma once
#include "../../engine/SubState.h"
#include "../../engine/Text.h"

class PauseSubState : public SubState {
public:
    PauseSubState();
    ~PauseSubState();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

private:
    Text* pauseText;
};
