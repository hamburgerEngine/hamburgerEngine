Blehhh:

```cpp
// Load an image
Sprite* playerSprite = Paths::image("player");

// Load a sound
Sound* jumpSound = Paths::sound("jump");

// Load a font
std::string fontPath = Paths::font("main");

// Load a music track
Sound* backgroundMusic = Paths::music("background");

// Load a text file
std::vector<std::string> lines = Paths::getText(Paths::txt("config"));

// Load a JSON file
std::string jsonContent = Paths::getTextFromFile(Paths::json("data"));
```