#pragma once

#include "Text.h"
#include <functional>

class Button : public Text {
public:
    Button(float x = 0, float y = 0, const std::string& text = "", std::function<void()> onClick = nullptr);
    ~Button() override;

    void update(float deltaTime) override;
    void render() override;

    void setOnClick(std::function<void()> callback) { onClick = callback; }
    void setBackgroundColor(unsigned int color) { backgroundColor = color; }
    void setHoverColor(unsigned int color) { hoverColor = color; }
    void setPadding(float padding) { this->padding = padding; }

    bool isHovered() const { return hovered; }
    bool isPressed() const { return pressed; }

private:
    void checkHover();
    void checkClick();

    std::function<void()> onClick;
    unsigned int backgroundColor;
    unsigned int hoverColor;
    float padding;
    bool hovered;
    bool pressed;
}; 