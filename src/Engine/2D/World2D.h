#pragma once
#include <functional>

#include "../Rendering/World.h"
#include "../Particle/Billboard.h"
#include "../Lightning/LightManager.h"
#include "../Physics/Physics.h"
#include "Layer/Layer.h"

class World2D : public World
{
private:
    std::vector<Layer*> vLayers;
	LightManager* pLightManager;

public:
    World2D();
    ~World2D();

    void update() override;
    void renderSky() override;

    LightManager* getLightManager();
    unsigned int numLayers() const;
    Layer* getSpriteLayer(const unsigned int& layer);
};