// #include "Renderer3D.h"

// #include "../General/World2D.h"
// #include "../Lightning/Lightning.h"
// #include "../Lightning/ShadowMapping/ShadowMapping.h"
// #include "../PostProcessing/PostProcessing.h"
// #include "../Particle/ShaderInitializer.h"
// #include "../Managers/TextureManager.h"
// #include "IDRenderer.h"

// #include <Desktop/Window.h>
// #include <System/MemoryManagement.h>

// Renderer3D::Renderer3D(Box* canvas, World2D* world)
// {
//     pRenderCanvas = canvas;
//     pWorld = world;

//     pRenderCanvas->invertTexcoordY(true);
// 	pGBuffer      = new G_Buffer(pRenderCanvas);
// 	pSSAO         = new SSAO(pRenderCanvas, pGBuffer, this);
// 	pLightning    = new Lightning(pRenderCanvas, this);
//     pShadowMaps   = new ShadowMapping(this);
//     #ifdef DEBUG
//     pGrid         = new Grid();
//     #endif

//     pIDRenderer = new IDRenderer(pRenderCanvas);

//     //Setting up Occlusion Culling
//     Gum::Output::log("Creating Occlusion Culling Object");
//     pOcclusionMask = new OcclusionMask(canvas->getSize().x, canvas->getSize().y);
// 	//pEnvironmentMap = new EnvironmentMap(pObjectManager);


//     pFramebuffer = new Framebuffer(canvas->getSize());
//     pFramebuffer->addTextureAttachment(0, "Renderer3DFB", GL_RGBA, GL_RGBA16F);
//     pFramebuffer->addDepthTextureAttachment();
//     //pFramebuffer->addDepthStencilTextureAttachment();

//     pHighDynamicRange = new HighDynamicRange(pRenderCanvas);
//     this->fExposure = 1.0f;

//     if(ActiveRenderer == nullptr)
//         ActiveRenderer = this;
// }

// Renderer3D::~Renderer3D()
// {
//     Gum::_delete(pOcclusionMask);
//     Gum::_delete(pFramebuffer);
// 	Gum::_delete(pGBuffer);
// 	Gum::_delete(pSSAO);
// 	Gum::_delete(pLightning);
//     Gum::_delete(pShadowMaps);
//     Gum::_delete(pHighDynamicRange);
// }


// void Renderer3D::render()
// {
// 	start = std::chrono::high_resolution_clock::now();

//     //GumEngine::DefaultOutlineRenderer->resetFramebuffer();

//     //Render Occlusion Mask via CPU
//     pOcclusionMask->render();
    
//     //Render Objects to GBuffer
//     pGBuffer->bind();
//     pGBuffer->getShader()->use();
//     pGBuffer->getShader()->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
//     pGBuffer->getShader()->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
//     pWorld->getObjectManager()->renderToGBuffer(pGBuffer->getShader());
//     pGBuffer->getShader()->unuse();
//     pGBuffer->unbind();

//     //SSAO
//     //pSSAO->render();
//     pFramebuffer->bind();
//     glClearColor(0.02, 0.31, 0.53,1);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//     pWorld->renderSky();

// 	//pShadowMaps->getResultTexture(0)->bind(16);
// 	//pEnvironmentMap->getTexture()->bind(15);
//     pLightning->render(pShadowMaps, pWorld);
// 	//pEnvironmentMap->getTexture()->unbind(15);
// 	//pShadowMaps->getResultTexture(0)->unbind(16);

//     pGBuffer->getFramebuffer()->blitDepthToOtherFramebuffer(pFramebuffer);
    
//     pFramebuffer->bind();
//     pWorld->getObjectManager()->renderExceptGBuffer(pGBuffer->getShader(), Camera::ActiveCamera);

//     #ifdef DEBUG
//         pGrid->render();
//     #endif

//     pWorld->renderRenderable();

// 	//Gum::Output::debug("Rendering ParticleSystem");
//     pWorld->renderParticles();
//     pWorld->renderBillboards();


//     /**
//         Outline rendering has to be done separately due to GBuffer
//     */
//     //TODO
//     pWorld->getPhysics()->drawDebug();
//     pFramebuffer->unbind();




//     //Apply postprocessing effects here
//     Texture* lastTex = pFramebuffer->getTextureAttachment();
//     //Texture* lastTex = pLightning->getFramebuffer()->getTextureAttachment();
//     //for(int i = 0; i < vPostProcessingEffects.size(); i++)
//     {
//     //    vPostProcessingEffects[i]->render(lastTex);
//     //    lastTex = vPostProcessingEffects[i]->getResultTexture();
//     }


//     pHighDynamicRange->render(lastTex, this->fExposure);
//     lastTex = pHighDynamicRange->getResultTexture();

//     glCullFace(GL_FRONT);
//     //glCullFace(GL_BACK);
//     //Render the Shadowmap
//     //Gum::Output::debug("Rendering to Shadowmap");
//     pShadowMaps->prepare(*pWorld->getLightManager()->getSun()->getDirection(), 0);
//     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
//     pWorld->getObjectManager()->render(ObjectManager::WITHOUTSKYBOX, pShadowMaps->getShader(), true);

//     //ShadowMaps->prepare(*Lights->getSun()->getDirection(), 1);
//     //Objects->render(0, ShadowMaps->getShader());
    
//     //ShadowMaps->prepare(*Lights->getSun()->getDirection(), 2);
//     //Objects->render(0, ShadowMaps->getShader());
//     pShadowMaps->finish();
//     glCullFace(GL_BACK);


//     //Renders scene with all objects inside
//     //Update the enviorment map
//     //pEnvironmentMap->render();

//     pRenderCanvas->setTexture(lastTex);
//     //pRenderCanvas->setTexture(pFramebuffer->getTextureAttachment());
//     //pRenderCanvas->setTexture(Gum::TextureManager::getTexture("/home/gumse/Projects/gumball/gum-engine-backup/examples/assets/textures/grass.jpg", true));
//     //Gum::Output::print(pRenderCanvas->getSize().toString());

// 	auto elapsed = std::chrono::high_resolution_clock::now() - start;
// 	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
// }

// void Renderer3D::renderIDs()
// {
//     pIDRenderer->bind();
//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glEnable(GL_DEPTH_TEST);
    
//     pIDRenderer->getMeshShader()->use();
//     pIDRenderer->getMeshShader()->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
//     pIDRenderer->getMeshShader()->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
//     pWorld->getObjectManager()->render(ObjectManager::WITHOUTSKYBOX, pIDRenderer->getMeshShader(), true);

//     pWorld->renderRenderableIDs();
// }

// void Renderer3D::update()
// {
//     pWorld->update();
// }

// void Renderer3D::updateFramebufferSize()
// {
//     pGBuffer->getFramebuffer()->setSize(pRenderCanvas->getSize());
//     pFramebuffer->setSize(pRenderCanvas->getSize());
//     pIDRenderer->setSize(pRenderCanvas->getSize());
//     if(Camera::ActiveCamera != nullptr)
//         Camera::ActiveCamera->updateProjection(pRenderCanvas->getSize());

//     pWorld->updateProjection();
// }


// void Renderer3D::addPostProcessingEffect(PostProcessingEffect* effect) { vPostProcessingEffects.push_back(effect); }

// //Getter
// long long Renderer3D::getExecutionTime() const                         { return this->microseconds; }
// SSAO* Renderer3D::getSSAO()                                            { return this->pSSAO; }
// G_Buffer* Renderer3D::getGBuffer()                                     { return this->pGBuffer; }
// EnvironmentMap* Renderer3D::getEnvironmentMap()                        { return this->pEnvironmentMap; }
// ShadowMapping* Renderer3D::getShadowMapping()                          { return this->pShadowMaps; }
// Box* Renderer3D::getRenderCanvas()                                     { return this->pRenderCanvas; }
// float Renderer3D::getExposure() const                                  { return this->fExposure; }
// World2D* Renderer3D::getWorld()                                          { return this->pWorld; }
// Framebuffer* Renderer3D::getFramebuffer()                              { return this->pFramebuffer; }
// IDRenderer* Renderer3D::getIDRenderer()                                { return this->pIDRenderer; }

// //Setter
// void Renderer3D::setExposure(const float& exposure)                    { this->fExposure = exposure; }
// void Renderer3D::setRenderCanvas(Box* canvas)                          { this->pRenderCanvas = canvas; }
// void Renderer3D::setWorld(World2D* world) 
// { 
//     this->pWorld = world; 
//     world->getLightManager()->addCallback([this](Light* light, int index) {
//         pLightning->loadLight(light, index);
//     });
// }