#pragma once
#include <functional>
#include <Graphics/Object3D.h>

#include "../Rendering/World.h"
#include "../Particle/Billboard.h"
#include "../Lightning/LightManager.h"
#include "Object/ObjectManager.h"
#include "../Physics/Physics.h"

class World3D : public World
{
private:
	ObjectManager* pObjectManager;
	LightManager* pLightManager;

public:
    World3D();
    ~World3D();

    void update() override;
    void renderSky() override;
    void saveToFile(const Gum::Filesystem::File& file) override;
    static World3D* readFromFile(XMLReader& reader);

    LightManager* getLightManager();
    ObjectManager* getObjectManager();
};