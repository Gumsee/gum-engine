#pragma once
#include "../2D/World2D.h"
#include "../3D/World3D.h"

class WorldLoader
{
public:
    static World* loadWorld(std::string filepath, std::string assetspath);
    static void saveWorld(std::string filepath, World* worldarg);
};