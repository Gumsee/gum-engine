#include "LightManager.h"
#include <Essentials/Output.h>
#include <Essentials/MemoryManagement.h>
#include "../General/Camera.h"
#include "../General/World.h"
#include "TextureManager.h"
#include "../Particle/Billboard.h"

LightManager::LightManager(World* world)
{
    //Gum::Output::log("Adding Lights!");
    this->pSun = new DirectionalLight(vec3(-1,-1,-1), vec3(1), "Sun");
    this->pWorld = world;

	//NearestPointLights.resize(Settings::getSetting(Settings::Names::NUM_ACTIVE_LIGHTS));
    for(size_t i = 0; i < NearestPointLights.size(); i++)
    {
	    NearestPointLights[i] = new PointLight(vec3(0,1000,0), vec3(0,0,0), "unused");
        PointLights.push_back(NearestPointLights[i]);
    }
    Gum::Output::info("Successfully initialized Light Manager!");
}

LightManager::~LightManager() 
{
    Gum::_delete(pSun);
    for(size_t i = 0; i < PointLights.size(); i++)
        Gum::_delete(PointLights[i]);
}

void LightManager::update()
{
    if(PointLights.size() > 0)
    {
        for(int i = 0; i < 4; i++)
        {
            NearestPointLights[i] = PointLights[i];
            std::cout << NearestPointLights[i]->getName() << std::endl;
            //NearestPointLights[i]->setPosition(PointLights[FinalIndex[i]]->getPosition());
            //NearestPointLights[i]->setColor(PointLights[FinalIndex[i]]->getColor());
            //NearestPointLights[i]->setAttenuation(PointLights[FinalIndex[i]]->getAttenuation());
        }
    }
}


void LightManager::addPointLight(PointLight* light)
{
    PointLights.push_back(light);

    Billboard* billboard = new Billboard(light->getPosition(), pWorld);
    light->setCallback([billboard, this](Light* clight) {
        billboard->setTexture(Gum::TextureManager::getTexture("lightbulb"));
        billboard->setPosition(clight->getPosition());

        for(int i = 0; i < vCallbackFunctions.size(); i++)
            vCallbackFunctions[i](clight);
    });
    pWorld->addBillboard(billboard);
}
void LightManager::addSpotLight(SpotLight* light)
{
	SpotLights.push_back(light);
}


void LightManager::addCallback(std::function<void(Light*)> callback)
{
    this->vCallbackFunctions.push_back(callback);
}


DirectionalLight *LightManager::getSun()
{
    return pSun;
}