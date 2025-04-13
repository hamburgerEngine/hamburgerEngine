#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text {
public:
    Text(float x = 0, float y = 0, int z = 0);
    virtual ~Text();
    
    void setFormat(const std::string& fontPath, int fontSize, unsigned int color);
    void setText(const std::string& text);
    void setPosition(float x, float y);
    virtual void update(float deltaTime);
    virtual void render();
    
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getLineHeight() const { return lineHeight; }
    void setLineSpacing(float spacing) { lineSpacing = spacing; }
    float getLineSpacing() const { return lineSpacing; }

    void setVisible(bool visible) { isVisible = visible; }
    bool getVisible() const { return isVisible; }

    TTF_Font* getFont() const { return font; }
    const std::string& getText() const { return text; }
    float getX() const { return x; }
    float getY() const { return y; }

protected:
    float x;
    float y;

private:
    void loadFont(const std::string& fontPath);
    void updateTexture();
    void renderText(const std::string& text, float x, float y);

    std::string text;
    float width;
    float height;
    float lineHeight;
    float lineSpacing;
    unsigned int color;
    int fontSize;
    TTF_Font* font;
    SDL_Texture* texture;
    bool isVisible;
};
