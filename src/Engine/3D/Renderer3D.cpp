#include "Renderer3D.h"

#include "Graphics/Graphics.h"
#include "Graphics/ShaderProgram.h"
#include "World3D.h"
#include "Lightning/Lightning.h"
#include "Lightning/ShadowMapping/ShadowMapping.h"
#include "../Rendering/IDRenderer.h"

#include <System/MemoryManagement.h>

Renderer3D::Renderer3D(Canvas* canvas)
    : Renderer(canvas, RENDERER3D)
{
    pWorld = nullptr;
    bRenderSky = true;
    pGBuffer      = new G_Buffer(pRenderCanvas->getSize());
    pSSAO         = new SSAO(pRenderCanvas, pGBuffer, this);
    pLightning    = new Lightning(pRenderCanvas);
    #ifndef GUM_ENGINE_NO_SHADOWMAP
    pShadowMaps   = new ShadowMapping();
    #endif
    //#ifdef DEBUG
    pGrid         = new Grid();
    //#endif

    pParticleShader = ShaderProgram::getShaderProgramByName("ParticleShader");
    pBillboardShader = ShaderProgram::getShaderProgramByName("BillboardShader");

    updateFramebufferSize();
}

Renderer3D::~Renderer3D()
{
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

    ShaderProgram::loadUniformForAll("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    ShaderProgram::loadUniformForAll("canvassize", pRenderCanvas->getSize());
    //GumEngine::DefaultOutlineRenderer->resetFramebuffer();

    //Render the Shadowmap
    #ifndef GUM_ENGINE_NO_SHADOWMAP
    pShadowMaps->render(*pWorld->getLightManager()->getSun()->getDirection(), [this](){
        pWorld->getObjectManager()->renderEverythingMeshesOnly();
    });
    #endif

    //SSAO
    //pSSAO->render();

    pFramebuffer->bind();
    pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH | Framebuffer::ClearFlags::STENCIL);
    if(bRenderSky)
      pWorld->renderSky();

    pLightning->updateShader(pShadowMaps, pWorld);
    pWorld->getObjectManager()->renderDefered(pGBuffer, pRenderCanvas);
    
    pFramebuffer->bind();
    pWorld->getObjectManager()->renderForward();

    //#ifdef DEBUG
    //pGrid->render();
    //#endif

    pWorld->renderRenderable();


    pParticleShader->use();
    pWorld->renderParticles();

    pBillboardShader->use();
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
void Renderer3D::renderSky(bool rendersky)      { this->bRenderSky = rendersky; }
void Renderer3D::setWorld(World3D* world) 
{ 
    this->pWorld = world; 
    world->getLightManager()->addCallback([this](Light* light, int index) {
        pLightning->loadLight(light, index);
    });
}