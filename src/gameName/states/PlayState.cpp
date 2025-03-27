#ifdef __MINGW32__
#include "gameName/states/PlayState.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "gameName/substates/PauseSubState.h"
#else
#include <states/PlayState.h>
#include <Engine.h>
#include <Input.h>
#include <substates/PauseSubState.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>

PlayState* PlayState::instance = nullptr;

PlayState::PlayState() : backgroundSprite(nullptr) {
    instance = this;
}

PlayState::~PlayState() {
    delete backgroundSprite;
    
    for (auto sprite : levelSprites) {
        delete sprite;
    }
    levelSprites.clear();
}

void PlayState::loadLevel(const std::string& levelPath) {
    try {
        Engine* engine = Engine::getInstance();
        
        // Clear existing level sprites from both lists
        for (auto sprite : levelSprites) {
            delete sprite;
        }
        levelSprites.clear();
        engine->clearSprites();
        
        // Re-add background sprite since we just cleared all sprites
        engine->addSprite(backgroundSprite);
        
        std::cout << "Loading project file..." << std::endl;
        project = ogmo::Project::create("assets/data/maps.ogmo");
        std::cout << "Project loaded successfully" << std::endl;

        std::cout << "Loading level file: " << levelPath << std::endl;
        std::ifstream levelFile(levelPath);
        if (!levelFile.is_open()) {
            throw std::runtime_error("Failed to open level file: " + levelPath);
        }
        std::stringstream levelBuffer;
        levelBuffer << levelFile.rdbuf();
        level = ogmo::Level::create(levelBuffer.str());

        // Set up callbacks
        level.onTileLayerLoaded = [this](const std::vector<int>& tiles, const ogmo::LayerDefinition& layer) {
            handleTileLayer(tiles, layer);
        };

        level.onEntityLayerLoaded = [this](const std::vector<ogmo::EntityDefinition>& entities, const ogmo::LayerDefinition& layer) {
            handleEntityLayer(entities, layer);
        };

        // Load the level
        level.load();
    } catch (const std::exception& e) {
        std::cerr << "Error in loadLevel: " << e.what() << std::endl;
        throw; // Re-throw to see where the abort happens
    }
}

void PlayState::handleTileLayer(const std::vector<int>& tiles, const ogmo::LayerDefinition& layer) {
    Engine* engine = Engine::getInstance();
    
    // Find the tileset for this layer
    auto tileset = project.getTilesetTemplate(layer.tileset.value_or(""));
    if (!tileset) return;

    // Convert relative path to assets path by removing "../" and adding "assets/"
    std::string imagePath = tileset->path;
    if (imagePath.substr(0, 3) == "../") {
        imagePath = "assets/" + imagePath.substr(3);
    }

    // Create sprites for each tile
    for (size_t i = 0; i < tiles.size(); i++) {
        if (tiles[i] == -1) continue; // Skip empty tiles

        int tileX = (i % layer.gridCellsX) * layer.gridCellWidth;
        int tileY = (i / layer.gridCellsX) * layer.gridCellHeight;

        // Create a sprite for this tile using the corrected path
        Sprite* tileSprite = new Sprite(imagePath);
        tileSprite->setPosition(tileX + layer.offsetX, tileY + layer.offsetY);
        engine->addSprite(tileSprite);
        levelSprites.push_back(tileSprite);
    }
}

void PlayState::handleEntityLayer(const std::vector<ogmo::EntityDefinition>& entities, const ogmo::LayerDefinition& layer) {
    Engine* engine = Engine::getInstance();

    /*
    for (const auto& entity : entities) {
        // Example: If entity name is "player", set player position
        if (entity.name == "player" && playerSprite) {
            playerSprite->setPosition(entity.x, entity.y);
        }
        // Add handling for other entity types as needed
    }
    */
}

void PlayState::create() {
    Engine* engine = Engine::getInstance();

    backgroundSprite = new Sprite("assets/images/background.png");
    engine->addSprite(backgroundSprite);

    // Load the level
    loadLevel("assets/data/maps/map1.json");
}

void PlayState::update(float deltaTime) {
    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    } else {
        // nun
    }
}

void PlayState::render() {
    backgroundSprite->render();

    // Render all level sprites
    for (auto sprite : levelSprites) {
        sprite->render();
    }

    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
}

void PlayState::destroy() {
    delete backgroundSprite;

    // Clean up level sprites
    for (auto sprite : levelSprites) {
        delete sprite;
    }
    levelSprites.clear();

    backgroundSprite = nullptr;
}

void PlayState::openSubState(SubState* subState) {
    std::cout << "PlayState::openSubState called" << std::endl;
    State::openSubState(subState);
}

void PlayState::keyPressed(unsigned char key) {
    if (key == 'p') {
        if (instance->_subStates.empty()) {
            PauseSubState* pauseSubState = new PauseSubState();
            instance->openSubState(pauseSubState);
        } else {
            instance->closeSubState();
        }
    }
}

void PlayState::specialKeyPressed(int key, int x, int y) {
    std::cout << "Special key pressed: " << key << std::endl;
    
    int mappedKey;
    switch(key) {
        case GLUT_KEY_UP:    mappedKey = 128; break;
        case GLUT_KEY_DOWN:  mappedKey = 129; break;
        case GLUT_KEY_LEFT:  mappedKey = 130; break;
        case GLUT_KEY_RIGHT: mappedKey = 131; break;
        default: return;
    }
    
    Input::handleKeyPress(mappedKey);
}
