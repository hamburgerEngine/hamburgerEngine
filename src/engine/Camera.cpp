#include "Camera.h"

Camera::Camera() {}

void Camera::begin() {
    if (!visible) return;
    
    glPushMatrix();
    glTranslatef(-x, -y, 0.0f);
    glScalef(zoom, zoom, 1.0f);
}

void Camera::end() {
    if (!visible) return;
    glPopMatrix();
}
