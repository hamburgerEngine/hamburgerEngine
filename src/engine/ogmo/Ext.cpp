#include "../engine/ogmo/Ext.h"

namespace ogmo {

ValueMap Ext::toValueMap(const nlohmann::json& values) {
    ValueMap map;
    for (auto it = values.begin(); it != values.end(); ++it) {
        if (it->is_boolean()) {
            map[it.key()] = it->get<bool>();
        }
        else if (it->is_number_integer()) {
            map[it.key()] = it->get<int>();
        }
        else if (it->is_number_float()) {
            map[it.key()] = it->get<float>();
        }
        else if (it->is_string()) {
            map[it.key()] = it->get<std::string>();
        }
    }
    return map;
}

StringMap Ext::toStringMap(const nlohmann::json& values) {
    StringMap map;
    for (auto it = values.begin(); it != values.end(); ++it) {
        if (it->is_string()) {
            map[it.key()] = it->get<std::string>();
        }
    }
    return map;
}

}
