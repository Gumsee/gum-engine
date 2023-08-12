#include "World3D.h"
#include "../Shaders/ShaderManager.h"
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include "Camera3D.h"


World3D::World3D()
    : World(WORLD3D)
{
    pLightManager = new LightManager(this); 
    pObjectManager = new ObjectManager(pLightManager->getSun()->getDirection()); 
    pPhysics = new Physics();
    //vCamera.push_back(new Camera(context->getSize(), this, context));
}

World3D::~World3D()
{
    Gum::_delete(pLightManager);
    Gum::_delete(pObjectManager);
    Gum::_delete(pPhysics);
    for(size_t i = 0; i < vCamera.size(); i++)
        Gum::_delete(vCamera[i]);
}

void World3D::update()
{
    pObjectManager->getSkybox()->update();
    pLightManager->update();
    pPhysics->update();
}

void World3D::renderSky()
{
    pObjectManager->getSkybox()->getShaderProgram()->use();
    pObjectManager->getSkybox()->getShaderProgram()->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pObjectManager->getSkybox()->getShaderProgram()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pObjectManager->getSkybox()->render();
    pObjectManager->getSkybox()->getShaderProgram()->unuse();
}


//
// Getter
//
LightManager* World3D::getLightManager()   { return this->pLightManager; }
ObjectManager* World3D::getObjectManager() { return this->pObjectManager; }