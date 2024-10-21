#include "../../include/engine/Text.h"
#include <iostream>

Text::Text(float x, float y, float width) : x(x), y(y), width(width), fontSize(12), color(0xFFFFFFFF) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        throw std::runtime_error("FreeType initialization failed");
    }
}

Text::~Text() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    for (auto& ch : Characters) {
        glDeleteTextures(1, &ch.second.TextureID);
    }
}

void Text::setText(const std::string& text) {
    this->text = text;
}

void Text::setFormat(const std::string& fontPath, int fontSize, unsigned int color) {
    this->fontPath = fontPath;
    this->fontSize = fontSize;
    this->color = color;
    loadFont();
}

void Text::loadFont() {
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned long c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph for character " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<unsigned long, Character>(c, character));
    }
}

void Text::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f((color >> 24) / 255.0f, 
              ((color >> 16) & 0xFF) / 255.0f, 
              ((color >> 8) & 0xFF) / 255.0f, 
              (color & 0xFF) / 255.0f);

    float x = this->x;
    float y = this->y + fontSize;

    for (unsigned char c : text) {
        if (Characters.find(c) == Characters.end()) {
            std::cerr << "Character not found: " << c << std::endl;
            continue;
        }

        Character ch = Characters[c];

        float xpos = x + ch.BearingX;
        float ypos = y - ch.BearingY;

        float w = ch.Width;
        float h = ch.Height;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos + h);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        x += (ch.Advance >> 6);
    }

    glDisable(GL_BLEND);
}
