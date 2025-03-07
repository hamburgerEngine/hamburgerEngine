#include <Text.h>
#include <iostream>

Text::Text(float x, float y, int z) 
    : x(x), 
      y(y), 
      width(0.0f), 
      height(0.0f),
      text(""),
      color(0xFFFFFFFF),
      fontSize(12),
      ft(nullptr),
      face(nullptr) {
    
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        throw std::runtime_error("FreeType initialization failed");
    }
}

Text::~Text() {
    if (face) {
        FT_Done_Face(face);
    }
    if (ft) {
        FT_Done_FreeType(ft);
    }
    
    for (const auto& pair : characters) {
        GLuint texID = pair.second.textureID;
        glDeleteTextures(1, &texID);
    }
}

void Text::setText(const std::string& text) {
    this->text = text;
}

void Text::setFormat(const std::string& fontPath, int fontSize, unsigned int color) {
    this->fontSize = fontSize;
    this->color = color;
    loadFont(fontPath);
}

void Text::loadFont(const std::string& fontPath) {
    if (face) {
        FT_Done_Face(face);
    }

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FontCharacter fontChar = {
            texture,
            static_cast<int>(face->glyph->bitmap.width),
            static_cast<int>(face->glyph->bitmap.rows),
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters[c] = fontChar;
    }
}

float Text::getLineHeight() const {
    return fontSize * lineSpacing;
}

void Text::render() {
    if (!isVisible) return;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f((color >> 24) / 255.0f, 
              ((color >> 16) & 0xFF) / 255.0f, 
              ((color >> 8) & 0xFF) / 255.0f, 
              (color & 0xFF) / 255.0f);

    float startX = x;
    float currentX = startX;
    float currentY = y + fontSize;
    
    for (unsigned char c : text) {
        if (c == '\n') {
            currentX = startX;
            currentY += getLineHeight();
            continue;
        }

        auto it = characters.find(c);
        if (it == characters.end()) {
            std::cerr << "Character not found: " << c << std::endl;
            continue;
        }

        const FontCharacter& ch = it->second;

        float charX = currentX + ch.bearingX;
        float charY = currentY - ch.bearingY;

        float w = ch.width;
        float h = ch.height;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(charX, charY);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(charX + w, charY);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(charX + w, charY + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(charX, charY + h);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        currentX += (ch.advance >> 6);
    }

    glDisable(GL_BLEND);
}

void Text::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void Text::update(float deltaTime) {}
