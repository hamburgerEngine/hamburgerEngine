#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glut.h>
#include <map>

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    int Width;          // Width of glyph
    int Height;         // Height of glyph
    int BearingX;       // Offset from baseline to left of glyph
    int BearingY;       // Offset from baseline to top of glyph
    int Advance;        // Offset to advance to next glyph
};

class Text {
public:
    Text(float x, float y, float width);
    ~Text();

    void setText(const std::string& text);
    const std::string& getText() const { return text; }
    void setFormat(const std::string& fontPath, int fontSize, unsigned int color);
    void render();
    float getWidth() const { return width; }
    void setPosition(float x, float y) { this->x = x; this->y = y; }
    void update(float deltaTime) {} 

private:
    float x, y, width;
    std::string text;
    std::string fontPath;
    int fontSize;
    unsigned int color;
    FT_Library ft;
    FT_Face face;
    std::map<char, Character> Characters;

    void loadFont();
};
