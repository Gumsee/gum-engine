#include "World.h"
#include "../Managers/ShaderManager.h"
#include "Engine/Physics/Physics.h"
#include "Essentials/Window.h"
#include "../Particle/ParticleSystem.h"


World::World(const ivec2& resolution, Gum::Window* context)
{
    pLightManager = new LightManager(this); 
    pObjectManager = new ObjectManager(pLightManager->getSun()->getDirection()); 
    pParticleShader = Gum::ShaderManager::getShaderProgram("ParticleShader");
    pBillboardShader = Gum::ShaderManager::getShaderProgram("BillboardShader");
    pPhysics = new Physics();
    vCamera.push_back(new Camera(context->getSize(), this, context));
}

World::~World()
{

}

void World::update()
{
    pObjectManager->getSkybox()->update();
    pLightManager->update();
    for (size_t i = 0; i < vUpdateables.size(); i++) { vUpdateables[i](); }
}

void World::renderRenderable()
{
    for (size_t i = 0; i < vRenderables.size(); i++) { vRenderables[i](); }
}

void World::renderSky()
{
    pObjectManager->getSkybox()->getShader()->use();
    pObjectManager->getSkybox()->getShader()->LoadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pObjectManager->getSkybox()->getShader()->LoadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
    pObjectManager->getSkybox()->render();
    pObjectManager->getSkybox()->getShader()->unuse();
}

void World::renderParticles()
{
    pParticleShader->use();
    pParticleShader->LoadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pParticleShader->LoadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
	for (size_t i = 0; i < vParticles.size(); i++)
	{
		vParticles[i]->update();
		vParticles[i]->render();
	}
	pParticleShader->unuse();
}

void World::renderBillboards()
{
	pBillboardShader->use();
    pBillboardShader->LoadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pBillboardShader->LoadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
	for (size_t i = 0; i < vBillboards.size(); i++) { vBillboards[i]->render(pBillboardShader); }
	pBillboardShader->unuse();
}

void World::addUpdateable(std::function<void()> updateable) { this->vUpdateables.push_back(updateable); }
void World::addRenderable(std::function<void()> renderable) { this->vRenderables.push_back(renderable); }
void World::addBillboard(Billboard* billboard)              { this->vBillboards.push_back(billboard); }

LightManager* World::getLightManager()                      { return this->pLightManager; }
ObjectManager* World::getObjectManager()                    { return this->pObjectManager; }
Camera* World::getCamera(const unsigned int& index)         { return this->vCamera[index]; }
Physics* World::getPhysics()                                { return this->pPhysics; }