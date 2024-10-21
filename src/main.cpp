#include "../include/engine/Engine.h"
#include "../include/engine/Text.h"
#include <iostream>

int main(int argc, char** argv) {
    Engine engine(800, 600, "PhloxEngine");

    Sprite* testSprite = new Sprite("assets/images/placeholder.png");
    testSprite->setPosition(100, 100);
    engine.addSprite(testSprite);

    engine.run();
    return 0;
}
