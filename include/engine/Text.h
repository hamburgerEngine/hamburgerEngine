#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glut.h>
#include <map>

class Text {
private:
    struct FontCharacter {
        GLuint textureID;
        int width;
        int height;
        int bearingX;
        int bearingY;
        unsigned int advance;
    };

public:
    Text(float x = 0, float y = 0, int z = 0);
    ~Text();
    
    void setFormat(const std::string& fontPath, int fontSize, unsigned int color);
    void setText(const std::string& text);
    void setPosition(float x, float y);
    void update(float deltaTime);
    void render();
    
    float getWidth() const { return width; }
    float getHeight() const { return height; }

private:
    void loadFont(const std::string& fontPath);

    std::map<char, FontCharacter> characters;
    std::string text;
    float x;
    float y;
    float width;
    float height;
    unsigned int color;
    int fontSize;
    FT_Library ft;
    FT_Face face;
};
