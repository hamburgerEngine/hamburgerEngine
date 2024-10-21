#include "../include/engine/Engine.h"
#include "../include/engine/Text.h"
#include <iostream>

int main(int argc, char** argv) {
    Engine engine(800, 600, "PhloxEngine");

    Sprite* testSprite = new Sprite("assets/images/placeholder.png");
    testSprite->setPosition(100, 100);
    //testSprite->setScale(0.5f, 0.5f);
    engine.addSprite(testSprite);

    Text* myText = new Text(100, 100, 500);
    myText->setText("Stay Funky FNF");
    myText->setFormat("assets/fonts/5by7.ttf", 36, 0xFFFFFFFF); 
    engine.addText(myText);

    engine.run();
    return 0;
}
