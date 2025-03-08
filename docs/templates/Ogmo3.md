# Ogmo3 Level Loading

## Usage

```cpp
// PlayState.h
class PlayState : public State {
private:
    ogmo::Project project;
    ogmo::Level level;
    std::vector<Sprite*> levelSprites;
};

// PlayState.cpp
void PlayState::create() {
    loadLevel("assets/data/maps/level1.json");
}

void PlayState::loadLevel(const std::string& levelPath) {
    project = ogmo::Project::create("assets/data/maps.ogmo");
    level = ogmo::Level::create(levelPath);
    
    level.onTileLayerLoaded = [this](const std::vector<int>& tiles, const ogmo::LayerDefinition& layer) {
        handleTileLayer(tiles, layer);
    };
    
    level.onEntityLayerLoaded = [this](const std::vector<ogmo::EntityDefinition>& entities, const ogmo::LayerDefinition& layer) {
        handleEntityLayer(entities, layer);
    };
    
    level.load();
}
```

## Setting Up Level Loading

Create handlers for both tile layers and entity layers:

```cpp
void PlayState::handleTileLayer(const std::vector<int>& tiles, const ogmo::LayerDefinition& layer) {
    auto tileset = project.getTilesetTemplate(layer.tileset.value_or(""));
    if (!tileset) return;

    std::string imagePath = tileset->path;
    if (imagePath.substr(0, 3) == "../") {
        imagePath = "assets/" + imagePath.substr(3);
    }

    for (size_t i = 0; i < tiles.size(); i++) {
        if (tiles[i] == -1) continue; 

        int tileX = (i % layer.gridCellsX) * layer.gridCellWidth;
        int tileY = (i / layer.gridCellsX) * layer.gridCellHeight;

        Sprite* tileSprite = new Sprite(imagePath);
        tileSprite->setPosition(tileX + layer.offsetX, tileY + layer.offsetY);
        engine->addSprite(tileSprite);
        levelSprites.push_back(tileSprite);
    }
}

void PlayState::handleEntityLayer(const std::vector<ogmo::EntityDefinition>& entities, const ogmo::LayerDefinition& layer) {
    for (const auto& entity : entities) {
        if (entity.name == "player") {
            playerSprite->setPosition(entity.x, entity.y);
        }
    }
}
```

## Available Properties

The Level class provides several properties from the Ogmo level file:

- `level.width`: Level width in pixels
- `level.height`: Level height in pixels
- `level.offsetX`: Level X offset
- `level.offsetY`: Level Y offset
- `level.layers`: Vector of layer definitions

Layer properties include:

- `layer.name`: Layer name
- `layer.gridCellWidth`: Tile width
- `layer.gridCellHeight`: Tile height
- `layer.gridCellsX`: Number of tiles horizontally
- `layer.gridCellsY`: Number of tiles vertically
- `layer.offsetX`: Layer X offset
- `layer.offsetY`: Layer Y offset
- `layer.tileset`: Tileset name (for tile layers)
- `layer.data`: Tile data (for tile layers)
- `layer.entities`: Entity data (for entity layers)

```cpp
// Example of accessing level properties
int levelWidth = level.width;
int levelHeight = level.height;

// Example of accessing layer properties
for (const auto& layer : level.layers) {
    std::cout << "Layer: " << layer.name << std::endl;
    std::cout << "Size: " << layer.gridCellsX << "x" << layer.gridCellsY << std::endl;
}
```
