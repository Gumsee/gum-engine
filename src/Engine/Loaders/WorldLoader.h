#pragma once
#include "../3D/World3D.h"

class WorldLoader
{
public:
    static World3D* loadWorld(std::string filepath, std::string assetspath);
    static void saveWorld(std::string filepath, World3D* world);
};