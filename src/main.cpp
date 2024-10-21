#include "../include/engine/Engine.h"
#include "../include/engine/Text.h"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {        
    Engine engine(800, 600, "PhloxEngine");

    Sprite* testSprite = new Sprite("assets/images/placeholder.png"); // asset path
    testSprite->setPosition(100, 100); // position
    testSprite->setScale(0.5f, 0.5f); // if you wanna set the scale (it's JUST like haxeflixel!)
    engine.addSprite(testSprite); // adds the sprite

    Text* myText = new Text(100, 100, 500);
    myText->setText("Stay Funky FNF");
    myText->setFormat("assets/fonts/5by7.ttf", 36, 0xFFFFFFFF); 
    //engine.addText(myText);

    engine.run();
    return 0;
}
