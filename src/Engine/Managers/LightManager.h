#pragma once
#include "../Lightning/DirectionalLight.h"
#include "../Lightning/SpotLight.h"
#include "../Lightning/PointLight.h"
#include <vector>

class World;

class LightManager
{
private:
    std::vector<PointLight*> PointLights, NearestPointLights;
    std::vector<SpotLight*> SpotLights, NearestSpotLights;
	DirectionalLight *pSun;
    World* pWorld;

public:
    LightManager(World* world);
    ~LightManager();

    void update();
    
    void addPointLight(PointLight* light);
    void addSpotLight(SpotLight* light);

    std::vector<PointLight*> getNearestPointLights();
    std::vector<SpotLight*> getNearestSpotLights();
    DirectionalLight *getSun();
};