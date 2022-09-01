#pragma once
#include "../General/World.h"

class WorldLoader
{
public:
    static World* loadWorld(std::string filepath, std::string assetspath);
    static void saveWorld(std::string filepath, World* world);
};