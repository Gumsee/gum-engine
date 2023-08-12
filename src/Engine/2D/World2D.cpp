#include "World2D.h"
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include "Camera2D.h"

World2D::World2D()
    : World(WORLD2D)
{
    pLightManager = new LightManager(this);
    pPhysics = new Physics();
    //vCamera.push_back(new Camera(context->getSize(), this, context));
}

World2D::~World2D()
{
    Gum::_delete(pLightManager);
    Gum::_delete(pPhysics);
    for(size_t i = 0; i < vCamera.size(); i++)
        Gum::_delete(vCamera[i]);
}

void World2D::update()
{
    pLightManager->update();
    pPhysics->update();
}

void World2D::renderSky()
{
}


//
// Getter
//
LightManager* World2D::getLightManager()                  { return this->pLightManager; }
unsigned int World2D::numLayers() const                   { return this->vLayers.size(); }
Layer* World2D::getSpriteLayer(const unsigned int& layer) { return this->vLayers[layer]; }