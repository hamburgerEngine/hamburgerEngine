#pragma once
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"

class PlayState : public State {
public:
    PlayState();
    ~PlayState();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

    void openSubState(SubState* subState);

    void keyPressed(unsigned char key) override;
    virtual void specialKeyPressed(int key, int x, int y) override;
    static PlayState* instance;

private:
    Sprite* backgroundSprite;
    AnimatedSprite* playerSprite;
};
