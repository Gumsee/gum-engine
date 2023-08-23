#pragma once
#include "../Lightning/DirectionalLight.h"
#include "../Lightning/SpotLight.h"
#include "../Lightning/PointLight.h"
#include <Graphics/Texture2D.h>
#include <vector>

class World;

#define GUM_MAX_LIGHTS 128

class LightManager
{
private:
    std::vector<PointLight*> PointLights;
    std::vector<SpotLight*> SpotLights;
    std::vector<std::function<void(Light*, int)>> vCallbackFunctions;
	DirectionalLight *pSun;
    static Texture2D* pLightbulb;


    World* pWorld;

public:
    LightManager(World* world);
    ~LightManager();

    static void cleanup();

    void update();
    
    void addPointLight(PointLight* light);
    void addSpotLight(SpotLight* light);
    void addCallback(std::function<void(Light*, int)> callback);


    unsigned int numPointLights();
    PointLight* getPointLight(const unsigned int& index);
    DirectionalLight *getSun();
};