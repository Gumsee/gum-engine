#pragma once
#include <functional>

#include "../Rendering/World.h"
#include "../Particle/Billboard.h"
#include "../Lightning/LightManager.h"
#include "../Physics/Physics.h"
#include "Codecs/XMLReader.h"
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
    void saveToFile(const Gum::Filesystem::File& file) override;
    static World2D* readFromFile(XMLReader& reader);


    void addLayer(Layer* layer);

    LightManager* getLightManager();
    unsigned int numLayers() const;
    Layer* getSpriteLayer(const unsigned int& layer);
};