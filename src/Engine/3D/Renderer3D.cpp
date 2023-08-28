#include "Renderer3D.h"

#include "Graphics/Graphics.h"
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
    pShadowMaps   = new ShadowMapping(this);
    #ifdef DEBUG
    pGrid         = new Grid();
    #endif

    pParticleShader = Gum::ShaderManager::getShaderProgram("ParticleShader");
    pBillboardShader = Gum::ShaderManager::getShaderProgram("BillboardShader");
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
    
    //Render Objects to GBuffer
    pGBuffer->bind();
    pGBuffer->getShader()->use();
    pGBuffer->getShader()->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pGBuffer->getShader()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pWorld->getObjectManager()->renderToGBuffer(pGBuffer->getShader());
    pGBuffer->getShader()->unuse();
    pGBuffer->unbind();

    //SSAO
    //pSSAO->render();
    pFramebuffer->bind();
    pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH | Framebuffer::ClearFlags::STENCIL);
    pWorld->renderSky();

	//pShadowMaps->getResultTexture(0)->bind(16);
	//pEnvironmentMap->getTexture()->bind(15);
    pLightning->render(pShadowMaps, pWorld);
	//pEnvironmentMap->getTexture()->unbind(15);
	//pShadowMaps->getResultTexture(0)->unbind(16);

    pGBuffer->getFramebuffer()->blitDepthToOtherFramebuffer(pFramebuffer);
    
    pFramebuffer->bind();
    pWorld->getObjectManager()->renderExceptGBuffer(pGBuffer->getShader(), Camera::getActiveCamera());

    #ifdef DEBUG
        pGrid->render();
    #endif

    pWorld->renderRenderable();


    pParticleShader->use();
    pParticleShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pParticleShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pWorld->renderParticles(pParticleShader);

	pBillboardShader->use();
    pBillboardShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pBillboardShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pWorld->renderBillboards(pBillboardShader);


    /**
        Outline rendering has to be done separately due to GBuffer
    */
    //TODO
    pWorld->getPhysics()->drawDebug();
    pFramebuffer->unbind();

    //Render the Shadowmap
    pShadowMaps->prepare(*pWorld->getLightManager()->getSun()->getDirection(), 0);
    Gum::Graphics::renderWireframe(false);
    pWorld->getObjectManager()->render(ObjectManager::WITHOUTSKYBOX, pShadowMaps->getShader(), true);

    //ShadowMaps->prepare(*Lights->getSun()->getDirection(), 1);
    //Objects->render(0, ShadowMaps->getShader());
    
    //ShadowMaps->prepare(*Lights->getSun()->getDirection(), 2);
    //Objects->render(0, ShadowMaps->getShader());
    pShadowMaps->finish();


    //Renders scene with all objects inside
    //Update the enviorment map
    //pEnvironmentMap->render();
}

void Renderer3D::renderIDsInternal()
{
    if(pWorld == nullptr)
        return;

    pWorld->getObjectManager()->render(ObjectManager::WITHOUTSKYBOX, pIDRenderer->getMeshShader(), true);
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

    if(pWorld != nullptr)
        pWorld->updateProjection();
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