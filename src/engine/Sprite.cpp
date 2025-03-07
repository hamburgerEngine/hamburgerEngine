#ifdef __MINGW32__
#include "engine/Sprite.h"
#include "engine/Camera.h"
#include <GL/glut.h>
#include "thirdparty/stb_image.h"
#include <iostream>
#else
#include <Sprite.h>
#include <Camera.h>
#include <GL/glut.h>
#include <stb_image.h>
#include <iostream>
#endif

Sprite::Sprite() 
    : imagePath("")
    , visible(true)  
{
}

Sprite::Sprite(const std::string& path) 
    : imagePath(path)
    , visible(true)
{
    loadTexture(path);  
}

Sprite::~Sprite() {
    if (imageData) {
        stbi_image_free(imageData);
        imageData = nullptr;
    }
    if (textureID) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void Sprite::render() {
    if (!visible) return; 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    
    if (camera) {
        glTranslatef(x, y, 0);
    } else {
        glTranslatef(x, y, 0);
    }
    
    glScalef(scale.x, scale.y, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(width, 0);
    glTexCoord2f(1, 1); glVertex2f(width, height);
    glTexCoord2f(0, 1); glVertex2f(0, height);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void Sprite::loadTexture(const std::string& imagePath) {
    int channels;
    imageData = stbi_load(imagePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!imageData) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

void Sprite::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}
