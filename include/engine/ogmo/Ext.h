#pragma once
#include <map>
#include <string>
#include <variant>
#include <json.hpp>

namespace ogmo {
    using ValueMap = std::map<std::string, std::variant<bool, int, float, std::string>>;
    using StringMap = std::map<std::string, std::string>;

    class Ext {
    public:
        static ValueMap toValueMap(const nlohmann::json& values);
        static StringMap toStringMap(const nlohmann::json& values);
    };
}