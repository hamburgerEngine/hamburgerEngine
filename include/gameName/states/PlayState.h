#pragma once
#include "../../engine/State.h"
#include "../../engine/Sprite.h"
#include "../../engine/AnimatedSprite.h"
#include "../../engine/ogmo/Project.h"
#include "../../engine/ogmo/Level.h"

void playStateKeyboardCallback(unsigned char key, int x, int y);

class PlayState : public State {
public:
    PlayState();
    ~PlayState();

    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

    void openSubState(SubState* subState);

    void keyPressed(unsigned char key) override;
    virtual void specialKeyPressed(int key, int x, int y) override;
    static PlayState* instance;

private:
    void loadLevel(const std::string& levelPath);
    void handleTileLayer(const std::vector<int>& tiles, const ogmo::LayerDefinition& layer);
    void handleEntityLayer(const std::vector<ogmo::EntityDefinition>& entities, const ogmo::LayerDefinition& layer);

    Sprite* backgroundSprite;
    
    ogmo::Project project;
    ogmo::Level level;
    std::vector<Sprite*> levelSprites; // To store level tiles/entities
};
