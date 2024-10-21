Simple Image/BG Sprite:
```cpp
    Sprite* testSprite = new Sprite("assets/images/placeholder.png"); // asset path
    testSprite->setPosition(100, 100); // position
    //testSprite->setScale(0.5f, 0.5f); // if you wanna set the scale (it's JUST like haxeflixel!)
    engine.addSprite(testSprite); // adds the sprite
```

Sprite with Animation (Spritesheet and XML):
```cpp
    AnimatedSprite* testSprite = new AnimatedSprite();
    testSprite->setPosition(100, 100); // x pos, y pos
    testSprite->loadFrames("assets/images/BOYFRIEND.png", "assets/images/BOYFRIEND.xml"); // png path, xml path
    testSprite->addAnimation("idle", "BF idle dance", 24, true); // animation name, prefix, frame rate, loop
    engine.addAnimatedSprite(testSprite); // ads the sprite
    testSprite->playAnimation("idle"); // animation name
    // no way it's JUST LIKE HAXEFLIXEL!
```
