#pragma once
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"

void playStateKeyboardCallback(unsigned char key, int x, int y);

class PlayState : public State {
public:
    PlayState();
    ~PlayState();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

    void openSubState(SubState* subState);

    static void keyPressed(unsigned char key, int x, int y);
    static void specialKeyPressed(int key, int x, int y);
    static PlayState* instance;

private:
    Sprite* backgroundSprite;
    AnimatedSprite* playerSprite;
};
