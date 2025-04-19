#pragma once

#include <string>
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "../graphics/Sprite.h"
#include "../audio/Sound.h"
#include "../graphics/AnimatedSprite.h"

class Paths {
public:
    static const std::string DEFAULT_FOLDER;

    static void initialize();
    static void cleanup();

    static std::string getPath(const std::string& folder, const std::string& file);
    static std::string file(const std::string& file, const std::string& folder = DEFAULT_FOLDER);
    static bool exists(const std::string& path);

    static std::vector<std::string> getText(const std::string& path);
    static std::string getTextFromFile(const std::string& key);
    static std::string txt(const std::string& key);
    static std::string json(const std::string& key);
    static std::string xml(const std::string& key);

    static std::string sound(const std::string& key);
    static std::string music(const std::string& key);

    static std::string font(const std::string& key);

    static std::string image(const std::string& key);

    static void clearUnusedMemory();
    static void clearStoredMemory();
    static void gc(bool major = false, int repeat = 1);
    static void compress();

private:
    static std::map<std::string, SDL_Texture*> currentTrackedAssets;
    static std::map<std::string, Sound*> currentTrackedSounds;
    static std::vector<std::string> localTrackedAssets;

    static Sprite* returnGraphic(const std::string& key, bool cache);
    static Sound* returnSound(const std::string& key, bool cache);
    static void destroyGraphic(Sprite* graphic);
}; 