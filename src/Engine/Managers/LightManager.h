#pragma once
#include "../Lightning/DirectionalLight.h"
#include "../Lightning/SpotLight.h"
#include "../Lightning/PointLight.h"
#include <vector>

class World;

#define GUM_MAX_LIGHTS 128

class LightManager
{
private:
    std::vector<PointLight*> PointLights, NearestPointLights;
    std::vector<SpotLight*> SpotLights, NearestSpotLights;
    std::vector<std::function<void(Light*)>> vCallbackFunctions;
	DirectionalLight *pSun;
    
    World* pWorld;

public:
    LightManager(World* world);
    ~LightManager();

    void update();
    
    void addPointLight(PointLight* light);
    void addSpotLight(SpotLight* light);
    void addCallback(std::function<void(Light*)> callback);

    std::vector<PointLight*> getNearestPointLights();
    std::vector<SpotLight*> getNearestSpotLights();
    DirectionalLight *getSun();
};