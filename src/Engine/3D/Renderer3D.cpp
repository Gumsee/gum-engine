#include "Renderer3D.h"

#include "Graphics/Graphics.h"
#include "System/Output.h"
#include "World3D.h"
#include "../Shaders/ShaderManager.h"
#include "Lightning/Lightning.h"
#include "Lightning/ShadowMapping/ShadowMapping.h"
#include "../PostProcessing/PostProcessing.h"
#include "../Particle/ShaderInitializer.h"
#include "../Texture/TextureManager.h"
#include "../Rendering/IDRenderer.h"

#include <Desktop/Window.h>
#include <System/MemoryManagement.h>

Renderer3D::Renderer3D(Box* canvas)
    : Renderer(canvas, RENDERER3D)
{
    pWorld = nullptr;
	pGBuffer      = new G_Buffer(pRenderCanvas);
	pSSAO         = new SSAO(pRenderCanvas, pGBuffer, this);
	pLightning    = new Lightning(pRenderCanvas, this);
    pShadowMaps   = new ShadowMapping();
    #ifdef DEBUG
    pGrid         = new Grid();
    #endif

    pParticleShader = Gum::ShaderManager::getShaderProgram("ParticleShader");
    pBillboardShader = Gum::ShaderManager::getShaderProgram("BillboardShader");

    updateFramebufferSize();
}

Renderer3D::~Renderer3D()
{
    Gum::_delete(pOcclusionMask);
    Gum::_delete(pFramebuffer);
	Gum::_delete(pGBuffer);
	Gum::_delete(pSSAO);
	Gum::_delete(pLightning);
    Gum::_delete(pShadowMaps);
    Gum::_delete(pHighDynamicRange);
}


void Renderer3D::renderInternal()
{
    if(pWorld == nullptr)
        return;
    //GumEngine::DefaultOutlineRenderer->resetFramebuffer();

    //Render the Shadowmap
    pShadowMaps->render(*pWorld->getLightManager()->getSun()->getDirection(), [this](){
        pWorld->getObjectManager()->renderEverything();
    });

    //SSAO
    //pSSAO->render();

    pFramebuffer->bind();
    pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH | Framebuffer::ClearFlags::STENCIL);
    pWorld->renderSky();
    pLightning->updateShader(pShadowMaps, pWorld);
    pWorld->getObjectManager()->renderDefered(pGBuffer, pRenderCanvas);
    
    pFramebuffer->bind();
    pWorld->getObjectManager()->renderForward();

    #ifdef DEBUG
        pGrid->render();
    #endif

    pWorld->renderRenderable();


    pParticleShader->use();
    pParticleShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pWorld->renderParticles(pParticleShader);

	pBillboardShader->use();
    pBillboardShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pWorld->renderBillboards(pBillboardShader);


    /**
        Outline rendering has to be done separately due to GBuffer
    */
    //TODO
    pWorld->getPhysics()->drawDebug();
    pFramebuffer->unbind();

    Gum::Graphics::renderWireframe(false);


    //Renders scene with all objects inside
    //Update the enviorment map
    //pEnvironmentMap->render();
}

void Renderer3D::renderIDsInternal()
{
    if(pWorld == nullptr)
        return;

    pIDRenderer->getMeshShader()->use();
    pIDRenderer->getMeshShader()->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pWorld->getObjectManager()->renderIDs();
    pWorld->renderRenderableIDs();
}

void Renderer3D::update()
{
    if(pWorld == nullptr)
        return;

    pWorld->update();
}

void Renderer3D::updateFramebufferSize()
{
    Renderer::updateFramebufferSize();
    pGBuffer->getFramebuffer()->setSize(pRenderCanvas->getSize());
    
    if(Camera::getActiveCamera() == nullptr)
        return;

    pGBuffer->getShader()->use();
    pGBuffer->getShader()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    
    pBillboardShader->use();
    pBillboardShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    
    pParticleShader->use();
    pParticleShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());

    pIDRenderer->getMeshShader()->use();
    pIDRenderer->getMeshShader()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    ShaderProgram::unuse();

    if(pWorld != nullptr)
    {
        pWorld->getObjectManager()->updateShaderPrograms(Camera::getActiveCamera());
        pWorld->updateProjection();
    }
}


//
// Getter
//
SSAO* Renderer3D::getSSAO()                     { return this->pSSAO; }
G_Buffer* Renderer3D::getGBuffer()              { return this->pGBuffer; }
EnvironmentMap* Renderer3D::getEnvironmentMap() { return this->pEnvironmentMap; }
ShadowMapping* Renderer3D::getShadowMapping()   { return this->pShadowMaps; }
World3D* Renderer3D::getWorld()                 { return this->pWorld; }

//
// Setter
//
void Renderer3D::setWorld(World3D* world) 
{ 
    this->pWorld = world; 
    world->getLightManager()->addCallback([this](Light* light, int index) {
        pLightning->loadLight(light, index);
    });
}