#include "LightManager.h"
#include <Essentials/Output.h>
#include <Essentials/MemoryManagement.h>
#include "../General/Camera.h"
#include "TextureManager.h"

LightManager::LightManager(World* world)
{
    //Gum::Output::log("Adding Lights!");
    this->pSun = new DirectionalLight(vec3(-1,-1,-1), vec3(1), "Sun");
    this->pWorld = world;

	NearestPointLights.resize(Settings::getSetting(Settings::Names::NUM_ACTIVE_LIGHTS));
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
        std::vector<int> FinalIndex;
        FinalIndex.resize(4);
        float NearestDistance = 500000;
        for (size_t i = 0; i < PointLights.size(); i++)
        {
            if (vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition()) < NearestDistance)
            {
                NearestDistance = vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition());
                FinalIndex[0] = i;
            }
        }
        NearestDistance = 500000;
        for (size_t i = 0; i < PointLights.size(); i++)
        {
            if ((int)i != FinalIndex[0])
            {
                if (vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition()) < NearestDistance)
                {
                    NearestDistance = vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition());
                    FinalIndex[1] = i;
                }
            }
        }
        NearestDistance = 500000;
        for (size_t i = 0; i < PointLights.size(); i++)
        {
            if ((int)i != FinalIndex[1] && (int)i != FinalIndex[0])
            {
                if (vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition()) < NearestDistance)
                {
                    NearestDistance = vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition());
                    FinalIndex[2] = i;
                }
            }
        }
        NearestDistance = 500000;
        for (size_t i = 0; i < PointLights.size(); i++)
        {
            if ((int)i != FinalIndex[2] && (int)i != FinalIndex[1] && (int)i != FinalIndex[0])
            {
                if (vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition()) < NearestDistance)
                {
                    NearestDistance = vec3::distance(PointLights[i]->getPosition(), Camera::ActiveCamera->getPosition());
                    FinalIndex[3] = i;
                }
            }
        }
        NearestPointLights[0]->setPosition(PointLights[FinalIndex[0]]->getPosition());
        NearestPointLights[1]->setPosition(PointLights[FinalIndex[1]]->getPosition());
        NearestPointLights[2]->setPosition(PointLights[FinalIndex[2]]->getPosition());
        NearestPointLights[3]->setPosition(PointLights[FinalIndex[3]]->getPosition());

        NearestPointLights[0]->setColor(PointLights[FinalIndex[0]]->getColor());
        NearestPointLights[1]->setColor(PointLights[FinalIndex[1]]->getColor());
        NearestPointLights[2]->setColor(PointLights[FinalIndex[2]]->getColor());
        NearestPointLights[3]->setColor(PointLights[FinalIndex[3]]->getColor());

        NearestPointLights[0]->setAttenuation(PointLights[FinalIndex[0]]->getAttenuation());
        NearestPointLights[1]->setAttenuation(PointLights[FinalIndex[1]]->getAttenuation());
        NearestPointLights[2]->setAttenuation(PointLights[FinalIndex[2]]->getAttenuation());
        NearestPointLights[3]->setAttenuation(PointLights[FinalIndex[3]]->getAttenuation());
    }
}


void LightManager::addPointLight(PointLight* light)
{
    PointLights.push_back(light);
	#ifdef DEBUG
        if(light->getName() != "unused")
        {
            Billboard* billboard = new Billboard(light->getPosition());
            billboard->setTexture(Gum::TextureManager::getTexture("lightbulb"));
            light->setBillboard(billboard);
            pWorld->addBillboard(billboard);
        }
	#endif
}
void LightManager::addSpotLight(SpotLight* light)
{
	SpotLights.push_back(light);
}

std::vector<PointLight*> LightManager::getNearestPointLights()
{
    return NearestPointLights;
}
std::vector<SpotLight*> LightManager::getNearestSpotLights()
{
    return NearestSpotLights;
}
DirectionalLight *LightManager::getSun()
{
    return pSun;
}