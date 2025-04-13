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
    if (Input::getInstance().isKeyPressed(SDLK_UP)) {
        sprite->playAnimation("up");
    } else if (Input::getInstance().isKeyPressed(SDLK_DOWN)) {
        sprite->playAnimation("down");
    } else if (Input::getInstance().isKeyPressed(SDLK_LEFT)) {
        sprite->playAnimation("left");
    } else if (Input::getInstance().isKeyPressed(SDLK_RIGHT)) {
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
SDL_Event event;
while (SDL_PollEvent(&event)) {
    Input::getInstance().handleEvent(event);
    // ... other event handling
}
Input::getInstance().update();
```

## Available Keys

You can handle any key supported by SDL2, including:

- A-Z and a-z
- 0-9
- Special keys like Space (SDLK_SPACE), Enter (SDLK_RETURN), Tab (SDLK_TAB)
- Arrow keys (SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT)
- Function keys (SDLK_F1 through SDLK_F12)
- Modifier keys (SDLK_LSHIFT, SDLK_RSHIFT, etc.)

## Mouse Input

The input system also supports mouse input:

```cpp
if (Input::getInstance().isMouseButtonPressed(SDL_BUTTON_LEFT)) {
    // Left mouse button is pressed
}

// mouse pos
int mouseX = Input::getInstance().getMouseX();
int mouseY = Input::getInstance().getMouseY();

// mouse movement
int deltaX = Input::getInstance().getMouseDeltaX();
int deltaY = Input::getInstance().getMouseDeltaY();
```

## Key States

The input system tracks three states for each key:

- `isKeyPressed`: Key is currently held down
- `isKeyJustPressed`: Key was pressed this frame
- `isKeyJustReleased`: Key was released this frame

Same states are available for mouse buttons:

- `isMouseButtonPressed`
- `isMouseButtonJustPressed`
- `isMouseButtonJustReleased`

## Controller Shiz

```cpp
Input::getInstance().isControllerButtonPressed(SDL_CONTROLLER_BUTTON_B) // controller button, can use isControllerButtonJustPressed, and isControllerButtonJustReleased by the way
Input::getInstance().getControllerAxis(SDL_CONTROLLER_AXIS_LEFTX)  // controller axis
```

Common button constants you can use:
```
SDL_CONTROLLER_BUTTON_A
SDL_CONTROLLER_BUTTON_B
SDL_CONTROLLER_BUTTON_X
SDL_CONTROLLER_BUTTON_Y
SDL_CONTROLLER_BUTTON_START
SDL_CONTROLLER_BUTTON_BACK
SDL_CONTROLLER_BUTTON_LEFTSHOULDER
SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
SDL_CONTROLLER_BUTTON_DPAD_UP
SDL_CONTROLLER_BUTTON_DPAD_DOWN
SDL_CONTROLLER_BUTTON_DPAD_LEFT
SDL_CONTROLLER_BUTTON_DPAD_RIGHT
SDL_CONTROLLER_BUTTON_START 
```
Common axis constants:
```
SDL_CONTROLLER_AXIS_LEFTX
SDL_CONTROLLER_AXIS_LEFTY
SDL_CONTROLLER_AXIS_RIGHTX
SDL_CONTROLLER_AXIS_RIGHTY
SDL_CONTROLLER_AXIS_TRIGGERLEFT
SDL_CONTROLLER_AXIS_TRIGGERRIGHT
```