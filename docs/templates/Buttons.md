yadda yadda, too lazy to write shit:

```cpp
Button* button = new Button(100, 100, "Click Me", []() { // width, height, text
    std::cout << "Button clicked!" << std::endl;
});

button->setBackgroundColor(0xFF0000FF); // bg color
button->setHoverColor(0xFF8080FF);      // hover color
button->setPadding(15.0f);              // text padding
button->setFormat("assets/fonts/yourfont.ttf", 24, 0xFFFFFFFF); // text format

button->update(deltaTime);
button->render();
```