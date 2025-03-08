#include "../engine/ogmo/Project.h"
#include <json.hpp>
#include <fstream>

namespace ogmo {

Project Project::create(const std::string& projectPath) {
    std::ifstream file(projectPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open project file: " + projectPath);
    }
    
    std::string jsonStr((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    try {
        nlohmann::json data = nlohmann::json::parse(jsonStr);
        
        Project project;
        project.name = data["name"].get<std::string>();
        project.backgroundColor = data["backgroundColor"].get<std::string>();
        project.gridColor = data["gridColor"].get<std::string>();
        project.anglesRadians = data["anglesRadians"].get<bool>();
        project.defaultExportMode = data.contains("defaultExportMode") ? 
            exportModeFromString(data["defaultExportMode"].get<std::string>()) : 
            ExportMode::JSON;
        project.directoryDepth = data["directoryDepth"].get<int>();
        
        for (const auto& path : data["levelPaths"]) {
            project.levelPaths.push_back(path.get<std::string>());
        }

        if (data.contains("tilesets")) {
            for (const auto& tileset : data["tilesets"]) {
                TilesetTemplate temp;
                temp.label = tileset["label"].get<std::string>();
                temp.path = tileset["path"].get<std::string>();
                temp.image = tileset["image"].get<std::string>();
                temp.tileWidth = tileset["tileWidth"].get<int>();
                temp.tileHeight = tileset["tileHeight"].get<int>();
                temp.tileSeparationX = tileset["tileSeparationX"].get<int>();
                temp.tileSeparationY = tileset["tileSeparationY"].get<int>();
                project.tilesets.push_back(temp);
            }
        }

        return project;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Failed to parse project JSON: " + std::string(e.what()));
    }
}

const TilesetTemplate* Project::getTilesetTemplate(const std::string& name) const {
    for (const auto& tileset : tilesets) {
        if (tileset.label == name) {
            return &tileset;
        }
    }
    return nullptr;
}

const LayerTemplate* Project::getLayerTemplate(const std::string& exportID) const {
    for (const auto& layer : layers) {
        if (layer.exportID == exportID) {
            return &layer;
        }
    }
    return nullptr;
}

const EntityTemplate* Project::getEntityTemplate(const std::string& exportID) const {
    for (const auto& entity : entities) {
        if (entity.exportID == exportID) {
            return &entity;
        }
    }
    return nullptr;
}

}
