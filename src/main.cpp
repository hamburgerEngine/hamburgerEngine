#include "../include/engine/Engine.h"
#include "../include/gameName/states/PlayState.h"
#include <GL/glut.h>

int main(int argc, char** argv) {
    Engine engine(800, 600, "PhloxEngine");

    PlayState* initialState = new PlayState();
    engine.pushState(initialState);

    glutKeyboardFunc(PlayState::keyPressed);
    glutSpecialFunc(PlayState::specialKeyPressed);

    engine.run();
    return 0;
}
