#include "Renderer3D.h"

#include "../General/World.h"
#include "../Lightning/Lightning.h"
#include "../Lightning/ShadowMapping/ShadowMapping.h"
#include "../PostProcessing/PostProcessing.h"
#include "../Particle/ShaderInitializer.h"
#include "../Managers/TextureManager.h"

#include <Essentials/Window.h>
#include <Essentials/MemoryManagement.h>

Renderer3D* Renderer3D::ActiveRenderer = nullptr;

Renderer3D::Renderer3D(Box* canvas, Gum::Window* context)
{
    pRenderCanvas = canvas;
    fAspectRatio = (float)pRenderCanvas->getSize().x / (float)pRenderCanvas->getSize().y;

    pRenderCanvas->invertTexcoordY(true);
	pGBuffer      = new G_Buffer(canvas->getSize());
	pSSAO         = new SSAO(pRenderCanvas, pGBuffer, this);
	pLightning    = new Lightning(pRenderCanvas, this);
    pShadowMaps   = new ShadowMapping(this);
    #ifdef DEBUG
    pGrid         = new Grid();
    #endif

    //Setting up Occlusion Culling
    Gum::Output::log("Creating Occlusion Culling Object");
    pOcclusionMask = new OcclusionMask(canvas->getSize().x, canvas->getSize().y);
	//pEnvironmentMap = new EnvironmentMap(pObjectManager);


    pFramebuffer = new Framebuffer(canvas->getSize());
    pFramebuffer->addTextureAttachment(0, "Renderer3DFB", GL_RGBA, GL_RGBA16F);
    pFramebuffer->addDepthAttachment();
    pFramebuffer->addDepthStencilTextureAttachment();

    pHighDynamicRange = new HighDynamicRange(pRenderCanvas);
    this->fExposure = 1.0f;

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


void Renderer3D::render()
{
	start = std::chrono::high_resolution_clock::now();

    //GumEngine::DefaultOutlineRenderer->resetFramebuffer();

    //Render Occlusion Mask via CPU
    pOcclusionMask->render();
    pWorld->renderRenderable();
    
    //Render Objects to GBuffer
    pGBuffer->bind();
    pGBuffer->getShader()->use();
    pGBuffer->getShader()->LoadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pGBuffer->getShader()->LoadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
    pWorld->getObjectManager()->renderToGBuffer(pGBuffer->getShader());
    pGBuffer->getShader()->unuse();
    pGBuffer->unbind();

    //SSAO
    //pSSAO->render();
    pFramebuffer->bind();
    glClearColor(0.02, 0.31, 0.53,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pWorld->renderSky();

	//pShadowMaps->getResultTexture(0)->bind(16);
	//pEnvironmentMap->getTexture()->bind(15);
    pLightning->render(pShadowMaps, pWorld);
	//pEnvironmentMap->getTexture()->unbind(15);
	//pShadowMaps->getResultTexture(0)->unbind(16);

    pGBuffer->getFramebuffer()->blitDepthToOtherFramebuffer(pFramebuffer);
    
    pFramebuffer->bind();
    pWorld->getObjectManager()->renderExceptGBuffer(pGBuffer->getShader(), Camera::ActiveCamera);

    #ifdef DEBUG
        pGrid->render();
    #endif

	Gum::Output::debug("Rendering ParticleSystem");
	pWorld->renderParticles();
    pWorld->renderBillboards();

    /**
        Outline rendering has to be done separately due to GBuffer
    */
    //TODO
    pFramebuffer->unbind();




    //Apply postprocessing effects here
    Texture* lastTex = pFramebuffer->getTextureAttachment();
    //Texture* lastTex = pLightning->getFramebuffer()->getTextureAttachment();
    //for(int i = 0; i < vPostProcessingEffects.size(); i++)
    {
    //    vPostProcessingEffects[i]->render(lastTex);
    //    lastTex = vPostProcessingEffects[i]->getResultTexture();
    }


    pHighDynamicRange->render(lastTex, this->fExposure);
    lastTex = pHighDynamicRange->getResultTexture();

    glCullFace(GL_FRONT);
    //glCullFace(GL_BACK);
    //Render the Shadowmap
    Gum::Output::debug("Rendering to Shadowmap");
    pShadowMaps->prepare(*pWorld->getLightManager()->getSun()->getDirection(), 0);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    pWorld->getObjectManager()->render(ObjectManager::WITHOUTSKYBOX, pShadowMaps->getShader(), true);

    //ShadowMaps->prepare(*Lights->getSun()->getDirection(), 1);
    //Objects->render(0, ShadowMaps->getShader());
    
    //ShadowMaps->prepare(*Lights->getSun()->getDirection(), 2);
    //Objects->render(0, ShadowMaps->getShader());
    pShadowMaps->finish();
    glCullFace(GL_BACK);


    //Renders scene with all objects inside
    //Update the enviorment map
    //pEnvironmentMap->render();

    pRenderCanvas->setTexture(lastTex);
    //pRenderCanvas->setTexture(Gum::TextureManager::getTexture("/home/gumse/Projects/gumball/gum-engine-backup/examples/assets/textures/grass.jpg", true));
    //Gum::Output::print(pRenderCanvas->getSize().toString());

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

void Renderer3D::update()
{
    pWorld->update();
}

void Renderer3D::updateFramebufferSize()
{
    fAspectRatio = (float)pRenderCanvas->getSize().x / (float)pRenderCanvas->getSize().y;
    //pGBuffer->getFramebuffer()->setSize(pRenderCanvas->getSize());
    //pFramebuffer->setSize(pRenderCanvas->getSize());
    Camera::ActiveCamera->updateProjection(pRenderCanvas->getSize());
}


void Renderer3D::addPostProcessingEffect(PostProcessingEffect* effect) { vPostProcessingEffects.push_back(effect); }


//Getter
long long Renderer3D::getExecutionTime() const                         { return this->microseconds; }
SSAO* Renderer3D::getSSAO()                                            { return this->pSSAO; }
G_Buffer* Renderer3D::getGBuffer()                                     { return this->pGBuffer; }
EnvironmentMap* Renderer3D::getEnvironmentMap()                        { return this->pEnvironmentMap; }
ShadowMapping* Renderer3D::getShadowMapping()                          { return this->pShadowMaps; }
Box* Renderer3D::getRenderCanvas()                                     { return this->pRenderCanvas; }
float Renderer3D::getExposure() const                                  { return this->fExposure; }
float Renderer3D::getAspectRatio() const                               { return this->fAspectRatio; }
World* Renderer3D::getWorld()                                          { return this->pWorld; }
Framebuffer* Renderer3D::getFramebuffer()                              { return this->pFramebuffer; }
Gum::Window* Renderer3D::getContextWindow()                            { return this->pContextWindow; }

//Setter
void Renderer3D::setExposure(const float& exposure)                    { this->fExposure = exposure; }
void Renderer3D::setRenderCanvas(Box* canvas)                          { this->pRenderCanvas = canvas; }
void Renderer3D::setWorld(World* world) 
{ 
    this->pWorld = world; 
    world->getLightManager()->addCallback([this](Light* light, int index) {
        pLightning->loadLight(light, index);
    });
}