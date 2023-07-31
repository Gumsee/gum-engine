// #include "World2D.h"
// #include "../Managers/ShaderManager.h"
// #include <Desktop/Window.h>
// #include "../Particle/ParticleSystem.h"
// #include <System/MemoryManagement.h>


// World2D::World2D()
// {
//     pLightManager = new LightManager(this); 
//     pObjectManager = new ObjectManager(pLightManager->getSun()->getDirection()); 
//     pParticleShader = Gum::ShaderManager::getShaderProgram("ParticleShader");
//     pBillboardShader = Gum::ShaderManager::getShaderProgram("BillboardShader");
//     pPhysics = new Physics();
//     //vCamera.push_back(new Camera(context->getSize(), this, context));
// }

// World2D::~World2D()
// {
//     Gum::_delete(pLightManager);
//     Gum::_delete(pObjectManager);
//     Gum::_delete(pPhysics);
//     for(size_t i = 0; i < vCamera.size(); i++)
//         Gum::_delete(vCamera[i]);
// }

// void World2D::update()
// {
//     pObjectManager->getSkybox()->update();
//     pLightManager->update();
//     pPhysics->update();
//     for (size_t i = 0; i < vUpdateables.size(); i++) { vUpdateables[i](); }
// }

// void World2D::renderRenderable()
// {
//     for (size_t i = 0; i < vRenderables.size(); i++) 
//     {
//         vRenderables[i]->prerender();
//         vRenderables[i]->render();
//     }
// }

// void World2D::renderRenderableIDs()
// {
//     for (size_t i = 0; i < vRenderables.size(); i++) 
//         vRenderables[i]->renderID();
// }

// void World2D::renderSky()
// {
//     pObjectManager->getSkybox()->getShaderProgram()->use();
//     pObjectManager->getSkybox()->getShaderProgram()->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
//     pObjectManager->getSkybox()->getShaderProgram()->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
//     pObjectManager->getSkybox()->render();
//     pObjectManager->getSkybox()->getShaderProgram()->unuse();
// }

// void World2D::renderParticles()
// {
//     pParticleShader->use();
//     pParticleShader->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
//     pParticleShader->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
// 	for (size_t i = 0; i < vParticles.size(); i++)
// 	{
// 		vParticles[i]->update();
// 		vParticles[i]->render();
// 	}
// 	pParticleShader->unuse();
// }

// void World2D::renderBillboards()
// {
// 	pBillboardShader->use();
//     pBillboardShader->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
//     pBillboardShader->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
// 	for (size_t i = 0; i < vBillboards.size(); i++) { vBillboards[i]->render(pBillboardShader); }
// 	pBillboardShader->unuse();
// }

// void World2D::updateProjection()
// {
//     for(Renderable* renderable : vRenderables)
//         renderable->onProjectionUpdate();
// }

// void World2D::addUpdateable(std::function<void()> updateable) { this->vUpdateables.push_back(updateable); }
// void World2D::addRenderable(Renderable* renderable)           { this->vRenderables.push_back(renderable); }
// void World2D::addBillboard(Billboard* billboard)              { this->vBillboards.push_back(billboard); }
// void World2D::addParticles(ParticleSystem* particles)         { this->vParticles.push_back(particles); }

// LightManager* World2D::getLightManager()                      { return this->pLightManager; }
// ObjectManager* World2D::getObjectManager()                    { return this->pObjectManager; }
// Camera* World2D::getCamera(const unsigned int& index)         { return this->vCamera[index]; }
// Physics* World2D::getPhysics()                                { return this->pPhysics; }