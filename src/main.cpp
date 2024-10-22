#include "../include/engine/Engine.h"
#include "../include/gameName/states/PlayState.h"
#include <iostream>

int main(int argc, char** argv) {        
    Engine engine(800, 600, "PhloxEngine");

    PlayState* initialState = new PlayState();
    engine.pushState(initialState);

    engine.run();
    return 0;
}