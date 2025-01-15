# Camera System

## Usage

```cpp
// PlayState.h
class PlayState : public State {
private:
    Camera camera;
};

// PlayState.cpp
void PlayState::render() {
    camera.begin();
    // All sprites rendered between begin() and end() will be affected by the camera
    sprite->render();
    camera.end();
}

// Move the camera
camera.setPosition(100, 100);  // Move camera to position
camera.x += 1.0f;             // Move camera right
camera.y -= 1.0f;             // Move camera up
camera.zoom = 2.0f;           // Zoom 
```

## Setting Up Camera

Create a Camera instance and use begin()/end() to define which objects should be affected by camera transformations:

```cpp
Camera camera;

void render() {
    camera.begin();
    // Objects here will be affected by camera position and zoom
    drawSprite();
    drawBackground();
    camera.end();
    
    // Objects here will be rendered in screen space (UI elements, etc.)
    drawHUD();
}
```

## Available Properties

The Camera class provides several properties you can modify:

- `x, y`: Camera position in world space
- `zoom`: Camera zoom level (1.0f is normal size)
- `visible`: Whether camera transformations should be applied

```cpp
camera.x = player->x - (screenWidth / 2);  // Center on player
camera.y = player->y - (screenHeight / 2);
camera.zoom = 0.5f;                        // Zoom out
camera.visible = false;                    // Disable camera temporarily
```