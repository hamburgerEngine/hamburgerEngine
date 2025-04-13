#ifdef __MINGW32__ || defined(__SWITCH__)
#include "Paths.h"
#include "SDLManager.h"
#include "SoundManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#else
#include <Paths.h>
#include <SDLManager.h>
#include <SoundManager.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#endif

const std::string Paths::DEFAULT_FOLDER = "assets";

std::map<std::string, SDL_Texture*> Paths::currentTrackedAssets;
std::map<std::string, Sound*> Paths::currentTrackedSounds;
std::vector<std::string> Paths::localTrackedAssets;

void Paths::initialize() {}

void Paths::cleanup() {
    clearStoredMemory();
}

std::string Paths::getPath(const std::string& folder, const std::string& file) {
    if (folder.empty()) {
        return DEFAULT_FOLDER + "/" + file;
    }
    return folder + "/" + file;
}

std::string Paths::file(const std::string& file, const std::string& folder) {
    if (folder != DEFAULT_FOLDER) {
        return getPath(folder, file);
    }
    return getPath("", file);
}

bool Paths::exists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::vector<std::string> Paths::getText(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }
    return lines;
}

std::string Paths::getTextFromFile(const std::string& key) {
    std::string path = file(key);
    std::ifstream file(path);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "";
}

std::string Paths::txt(const std::string& key) {
    return file(key + ".txt");
}

std::string Paths::json(const std::string& key) {
    return file(key + ".json");
}

std::string Paths::xml(const std::string& key) {
    return file(key + ".xml");
}

std::string Paths::sound(const std::string& key) {
    return file("sounds/" + key + ".ogg");
}

std::string Paths::music(const std::string& key) {
    return file("music/" + key + ".ogg");
}

std::string Paths::font(const std::string& key) {
    std::string path = file("fonts/" + key);
    if (path.substr(path.length() - 4) != ".ttf" && path.substr(path.length() - 4) != ".otf") {
        if (exists(path + ".ttf")) {
            return path + ".ttf";
        } else if (exists(path + ".otf")) {
            return path + ".otf";
        }
    }
    return path;
}

std::string Paths::image(const std::string& key) {
    return file("images/" + key + ".png");
}

void Paths::clearUnusedMemory() {
    for (auto it = currentTrackedAssets.begin(); it != currentTrackedAssets.end();) {
        if (std::find(localTrackedAssets.begin(), localTrackedAssets.end(), it->first) == localTrackedAssets.end()) {
            SDL_DestroyTexture(it->second);
            it = currentTrackedAssets.erase(it);
        } else {
            ++it;
        }
    }
    
    for (auto it = currentTrackedSounds.begin(); it != currentTrackedSounds.end();) {
        if (std::find(localTrackedAssets.begin(), localTrackedAssets.end(), it->first) == localTrackedAssets.end()) {
            delete it->second;
            it = currentTrackedSounds.erase(it);
        } else {
            ++it;
        }
    }
    
    compress();
    gc(true);
}

void Paths::clearStoredMemory() {
    for (auto& pair : currentTrackedAssets) {
        SDL_DestroyTexture(pair.second);
    }
    currentTrackedAssets.clear();
    
    for (auto& pair : currentTrackedSounds) {
        delete pair.second;
    }
    currentTrackedSounds.clear();
    
    localTrackedAssets.clear();
    gc(true);
    compress();
}

void Paths::gc(bool major, int repeat) {}

void Paths::compress() {}

void Paths::destroyGraphic(Sprite* graphic) {
    if (graphic) {
        delete graphic;
    }
} 