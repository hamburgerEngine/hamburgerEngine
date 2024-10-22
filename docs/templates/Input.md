# Input System

## Usage

```cpp
// PlayState.h
class PlayState : public State {
public:
    static void keyPressed(unsigned char key, int x, int y);
    static PlayState* instance;
};

// PlayState.cpp
void PlayState::keyPressed(unsigned char key, int x, int y) {
    if (key == 'p') {
        std::cout << "P key pressed in PlayState" << std::endl;
        if (instance->_subStates.empty()) {
            PauseSubState* pauseSubState = new PauseSubState();
            instance->openSubState(pauseSubState);
        } else {
            instance->closeSubState();
        }
    }
}
```

## Setting Up Input Handling

In your `main.cpp` file, set up the GLUT keyboard callback to use the state's `keyPressed` method:

```cpp
void keyboardCallback(unsigned char key, int x, int y) {
    PlayState::keyPressed(key, x, y);
}

int main(int argc, char** argv) {
    glutKeyboardFunc(keyboardCallback);
}
```

## Available Keys

You can handle any key supported by GLUT, including:

- A-Z and a-z
- 0-9
- Special keys like Space, Enter, Tab, etc.
- Arrow keys

For regular keys (like letters and numbers), use `keyPressed`. For special keys (arrow keys), you need to implement a separate method and use a different GLUT callback. Here's how to handle arrow keys:

```cpp
// PlayState.h
class PlayState : public State {
public:
    static void keyPressed(unsigned char key, int x, int y);
    static void specialKeyPressed(int key, int x, int y);
    static PlayState* instance;
};

// PlayState.cpp
void PlayState::specialKeyPressed(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            std::cout << "Up arrow pressed" << std::endl;
            break;
        case GLUT_KEY_DOWN:
            std::cout << "Down arrow pressed" << std::endl;
            break;
        case GLUT_KEY_LEFT:
            std::cout << "Left arrow pressed" << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            std::cout << "Right arrow pressed" << std::endl;
            break;
    }
}
```

In your `main.cpp`, set up both keyboard callbacks:

```cpp
glutKeyboardFunc(PlayState::keyPressed);
glutSpecialFunc(PlayState::specialKeyPressed);
```