# Input System

## Usage

```cpp
// PlayState.h
class PlayState : public State {
public:
    static void keyPressed(unsigned char key, int x, int y);
    static void specialKeyPressed(int key, int x, int y);
    static PlayState* instance;
};

// PlayState.cpp
void PlayState::update(float deltaTime) {
    if (Input::pressed(128)) {         // Up Arrow
        sprite->playAnimation("up");
    } else if (Input::pressed(129)) {  // Down Arrow
        sprite->playAnimation("down");
    } else if (Input::pressed(130)) {  // Left Arrow
        sprite->playAnimation("left");
    } else if (Input::pressed(131)) {  // Right Arrow
        sprite->playAnimation("right");
    } else {
        sprite->playAnimation("idle");
    }
}

void PlayState::specialKeyPressed(int key, int x, int y) {
    int mappedKey;
    switch(key) {
        case GLUT_KEY_UP:    mappedKey = 128; break;
        case GLUT_KEY_DOWN:  mappedKey = 129; break;
        case GLUT_KEY_LEFT:  mappedKey = 130; break;
        case GLUT_KEY_RIGHT: mappedKey = 131; break;
        default: return;
    }
    Input::handleKeyPress(mappedKey);
}
```

## Setting Up Input Handling

In your `main.cpp` file, set up both regular and special key callbacks:

```cpp
glutKeyboardFunc(Engine::keyboardCallback);
glutKeyboardUpFunc(Engine::keyboardUpCallback);
glutSpecialFunc(Engine::specialKeyCallback);
glutSpecialUpFunc(Engine::specialKeyUpCallback);
```

## Available Keys

You can handle any key supported by GLUT, including:

- A-Z and a-z
- 0-9
- Special keys like Space, Enter, Tab, etc.
- Arrow keys (mapped to 128-131)

For regular keys (like letters and numbers), use `keyPressed`. For special keys (arrow keys), you need to implement `specialKeyPressed` and map them to the correct values:

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
    int mappedKey;
    switch(key) {
        case GLUT_KEY_UP:    mappedKey = 128; break;
        case GLUT_KEY_DOWN:  mappedKey = 129; break;
        case GLUT_KEY_LEFT:  mappedKey = 130; break;
        case GLUT_KEY_RIGHT: mappedKey = 131; break;
        default: return;
    }
    Input::handleKeyPress(mappedKey);
}
```