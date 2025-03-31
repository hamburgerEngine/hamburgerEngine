#pragma once
#include <GL/glut.h>

class Camera {
public:
    Camera();
    void begin();
    void end();
    
    float x = 0.0f;
    float y = 0.0f;
    float zoom = 1.0f;
    bool visible = true;

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }
};
