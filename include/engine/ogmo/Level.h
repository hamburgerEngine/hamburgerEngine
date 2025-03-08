#pragma once
#include "Types.h"
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <json.hpp>
#include <map>
#include <variant>

namespace ogmo {
    using ValueMap = std::map<std::string, std::variant<bool, int, float, std::string>>;

    struct Point {
        float x;
        float y;
    };

    struct EntityDefinition {
        std::string name;
        int id;
        std::string exportID;
        float x;
        float y;
        std::optional<float> width;
        std::optional<float> height;
        std::optional<float> originX;
        std::optional<float> originY;
        std::optional<float> rotation;
        std::optional<bool> flippedX;
        std::optional<bool> flippedY;
        std::optional<std::vector<Point>> nodes;
        std::optional<ValueMap> values;
    };

    struct DecalDefinition {
        float x;
        float y;
        std::string texture;
        std::optional<float> rotation;
        std::optional<float> scaleX;
        std::optional<float> scaleY;
    };

    struct LayerDefinition {
        std::string name;
        std::string exportID;
        float offsetX;
        float offsetY;
        int gridCellWidth;
        int gridCellHeight;
        int gridCellsX;
        int gridCellsY;
        std::optional<std::vector<int>> data;
        std::optional<std::vector<std::vector<int>>> data2D;
        std::optional<std::vector<std::vector<int>>> dataCoords;
        std::optional<std::vector<std::vector<std::vector<int>>>> dataCoords2D;
        std::optional<std::vector<std::string>> grid;
        std::optional<std::vector<std::vector<std::string>>> grid2D;
        std::optional<std::vector<EntityDefinition>> entities;
        std::optional<std::vector<DecalDefinition>> decals;
        std::optional<ExportMode> exportMode;
        std::optional<ArrayMode> arrayMode;
        std::optional<std::string> tileset;
        std::optional<std::string> folder;
    };

    class Level {
    public:
        Level() : width(0), height(0), offsetX(0), offsetY(0) {}
        static Level create(const std::string& json);
        void load();

        float width;
        float height;
        float offsetX;
        float offsetY;
        std::vector<LayerDefinition> layers;
        ValueMap values;

        std::function<void(const std::vector<DecalDefinition>&, const LayerDefinition&)> onDecalLayerLoaded;
        std::function<void(const std::vector<EntityDefinition>&, const LayerDefinition&)> onEntityLayerLoaded;
        std::function<void(const std::vector<std::string>&, const LayerDefinition&)> onGridLayerLoaded;
        std::function<void(const std::vector<std::vector<std::string>>&, const LayerDefinition&)> onGrid2DLayerLoaded;
        std::function<void(const std::vector<int>&, const LayerDefinition&)> onTileLayerLoaded;
        std::function<void(const std::vector<std::vector<int>>&, const LayerDefinition&)> onTile2DLayerLoaded;
        std::function<void(const std::vector<std::vector<int>>&, const LayerDefinition&)> onTileCoordsLayerLoaded;
        std::function<void(const std::vector<std::vector<std::vector<int>>>&, const LayerDefinition&)> onTileCoords2DLayerLoaded;
    };
}