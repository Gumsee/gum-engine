#include "LightManager.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include "../Rendering/Camera.h"
#include "../3D/World3D.h"
#include "../Particle/Billboard.h"
#include "../Lightning/lightbulb.h"

Texture2D* LightManager::pLightbulb = nullptr;

LightManager::LightManager(World* world)
{
    //Gum::Output::log("Adding Lights!");
    this->pSun = new DirectionalLight(vec3(-1,-1,-1), vec3(1), "Sun");
    this->pWorld = world;
    //Gum::Output::print("Successfully initialized Light Manager!");

    if(pLightbulb == nullptr)
    {
        pLightbulb = new Texture2D("lightbulb");
        pLightbulb->loadFromMemory(lightbulb, sizeof(lightbulb));
    }
}

LightManager::~LightManager() 
{
    Gum::_delete(pSun);
    for(size_t i = 0; i < PointLights.size(); i++)
        Gum::_delete(PointLights[i]);
}

void LightManager::cleanup()
{
    Gum::_delete(pLightbulb);
}

void LightManager::update()
{
    /*if(PointLights.size() > 0)
    {
        for(int i = 0; i < 4; i++)
        {
            NearestPointLights[i] = PointLights[i];
            std::cout << NearestPointLights[i]->getName() << std::endl;
            //NearestPointLights[i]->setPosition(PointLights[FinalIndex[i]]->getPosition());
            //NearestPointLights[i]->setColor(PointLights[FinalIndex[i]]->getColor());
            //NearestPointLights[i]->setAttenuation(PointLights[FinalIndex[i]]->getAttenuation());
        }
    }*/
}


void LightManager::addPointLight(PointLight* light)
{
    int index = numPointLights();
    PointLights.push_back(light);

    Billboard* billboard = new Billboard(light->getPosition());
    light->setCallback([billboard, this, index](Light* clight) {
        billboard->setTexture(pLightbulb);
        billboard->setPosition(clight->getPosition());
        billboard->setPixelSize(pLightbulb->getSize() * 0.2f);

        for(size_t i = 0; i < vCallbackFunctions.size(); i++)
        {
            vCallbackFunctions[i](clight, index);
        }
    });
    pWorld->addBillboard(billboard);
}
void LightManager::addSpotLight(SpotLight* light)
{
	SpotLights.push_back(light);
}


void LightManager::addCallback(std::function<void(Light*, int)> callback)
{
    this->vCallbackFunctions.push_back(callback);
    for(size_t i = 0; i < PointLights.size(); i++)
        PointLights[i]->update();
}

unsigned int LightManager::numPointLights()
{
    return PointLights.size();
}

PointLight* LightManager::getPointLight(const unsigned int& index)
{
    return this->PointLights[index];
}

DirectionalLight *LightManager::getSun()
{
    return pSun;
}