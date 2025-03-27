#include "../engine/ogmo/Level.h"
#include "../engine/ogmo/Ext.h"
#include <json.hpp>

namespace ogmo {

Level Level::create(const std::string& jsonStr) {
    nlohmann::json data = nlohmann::json::parse(jsonStr);
    
    Level level;
    level.width = data["width"].get<float>();
    level.height = data["height"].get<float>();
    level.offsetX = data["offsetX"].get<float>();
    level.offsetY = data["offsetY"].get<float>();

    if (data.contains("values")) {
        level.values = Ext::toValueMap(data["values"]);
    }

    if (data.contains("layers")) {
        for (const auto& layerData : data["layers"]) {
            LayerDefinition layer;
            layer.name = layerData["name"].get<std::string>();
            layer.exportID = layerData["_eid"].get<std::string>();
            layer.offsetX = layerData["offsetX"].get<float>();
            layer.offsetY = layerData["offsetY"].get<float>();
            layer.gridCellWidth = layerData["gridCellWidth"].get<int>();
            layer.gridCellHeight = layerData["gridCellHeight"].get<int>();
            layer.gridCellsX = layerData["gridCellsX"].get<int>();
            layer.gridCellsY = layerData["gridCellsY"].get<int>();

            if (layerData.contains("data")) {
                layer.data = layerData["data"].get<std::vector<int>>();
            }
            if (layerData.contains("tileset")) {
                layer.tileset = layerData["tileset"].get<std::string>();
            }
            if (layerData.contains("entities")) {
                std::vector<EntityDefinition> entities;
                for (const auto& entityData : layerData["entities"]) {
                    EntityDefinition entity;
                    entity.name = entityData["name"].get<std::string>();
                    entity.id = entityData["id"].get<int>();
                    entity.exportID = entityData["_eid"].get<std::string>();
                    entity.x = entityData["x"].get<float>();
                    entity.y = entityData["y"].get<float>();
                    
                    if (entityData.contains("width")) entity.width = entityData["width"].get<float>();
                    if (entityData.contains("height")) entity.height = entityData["height"].get<float>();
                    if (entityData.contains("originX")) entity.originX = entityData["originX"].get<float>();
                    if (entityData.contains("originY")) entity.originY = entityData["originY"].get<float>();
                    if (entityData.contains("rotation")) entity.rotation = entityData["rotation"].get<float>();
                    if (entityData.contains("flippedX")) entity.flippedX = entityData["flippedX"].get<bool>();
                    if (entityData.contains("flippedY")) entity.flippedY = entityData["flippedY"].get<bool>();
                    if (entityData.contains("values")) entity.values = Ext::toValueMap(entityData["values"]);
                    
                    entities.push_back(entity);
                }
                layer.entities = entities;
            }
            
            level.layers.push_back(layer);
        }
    }

    return level;
}

void Level::load() {
    for (const auto& layer : layers) {
        if (layer.data && onTileLayerLoaded) {
            onTileLayerLoaded(layer.data.value(), layer);
        }
        if (layer.entities && onEntityLayerLoaded) {
            onEntityLayerLoaded(layer.entities.value(), layer);
        }
    }
}

}
