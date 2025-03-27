#pragma once
#include <string>

enum class ExportMode {
    JSON,
    BINARY,
    CSV
};

inline ExportMode exportModeFromString(const std::string& str) {
    if (str == "JSON") return ExportMode::JSON;
    if (str == "BINARY") return ExportMode::BINARY;
    if (str == "CSV") return ExportMode::CSV;
    return ExportMode::JSON;
}

enum class ArrayMode {
    ONE,
    TWO
};

enum class ValueDefinition {
    BOOL,
    COLOR,
    ENUM,
    INT,
    FLOAT,
    STRING,
    TEXT
};

enum class LayerValueDefinition {
    DECAL,
    GRID,
    TILE,
    ENTITY
};
