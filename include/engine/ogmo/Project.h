#pragma once
#include "Types.h"
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <map>

namespace ogmo {
    struct ValueTemplate {
        std::string name;
        ValueDefinition definition;
        std::variant<bool, int, float, std::string> defaults;
        std::optional<bool> bounded;
        std::optional<float> min;
        std::optional<float> max;
        std::optional<int> maxLength;
        std::optional<bool> trimWhitespace;
        std::optional<std::vector<std::string>> choices;
        std::optional<bool> includeAlpha;
    };

    struct LayerTemplate {
        std::string name;
        LayerValueDefinition definition;
        struct { int x; int y; } gridSize;
        std::string exportID;
        std::optional<ExportMode> exportMode;
        std::optional<ArrayMode> arrayMode;
        std::optional<std::string> defaultTileset;
        std::optional<std::map<std::string, std::string>> legend;
        std::optional<std::vector<std::string>> requiredTags;
        std::optional<std::vector<std::string>> excludedTags;
    };

    struct EntityTemplate {
        std::string name;
        std::string exportID;
        int width;
        int height;
        int originX;
        int originY;
        bool rotatable;
        bool rotateIncrement;
        std::vector<std::string> tags;
        std::vector<ValueTemplate> values;
        std::string color;
        std::optional<std::string> tileX;
        std::optional<std::string> tileY;
        std::optional<std::string> tileset;
        std::optional<bool> resizableX;
        std::optional<bool> resizableY;
        std::optional<int> minWidth;
        std::optional<int> maxWidth;
        std::optional<int> minHeight;
        std::optional<int> maxHeight;
        std::optional<bool> rotationDegrees;
    };

    struct TilesetTemplate {
        std::string label;
        std::string path;
        std::string image;
        int tileWidth;
        int tileHeight;
        int tileSeparationX;
        int tileSeparationY;
    };

    class Project {
    public:
        static Project create(const std::string& projectPath);
        const LayerTemplate* getLayerTemplate(const std::string& name) const;
        const EntityTemplate* getEntityTemplate(const std::string& name) const;
        const TilesetTemplate* getTilesetTemplate(const std::string& name) const;

        std::string name;
        std::vector<std::string> levelPaths;
        ExportMode defaultExportMode;
        std::string backgroundColor;
        std::string gridColor;
        int directoryDepth;
        bool anglesRadians;

    private:
        std::vector<ValueTemplate> levelValues;
        std::vector<LayerTemplate> layers;
        std::vector<EntityTemplate> entities;
        std::vector<TilesetTemplate> tilesets;
    };
}