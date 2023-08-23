#include "Renderer2D.h"
#include "System/Output.h"
#include "World2D.h"
#include "../Rendering/Camera.h"
#include "../Shaders/ShaderManager.h"
#include "../Shaders/ParallaxSkyShader.h"

#include <System/MemoryManagement.h>

Renderer2D::Renderer2D(Box* canvas)
    : Renderer(canvas, RENDERER2D)
{
    pWorld = nullptr;
	pGBuffer = new G_Buffer(pRenderCanvas);

    initShader();

    pParticleShader = Gum::ShaderManager::getShaderProgram("ParticleShader");
    pBillboardShader = Gum::ShaderManager::getShaderProgram("BillboardShader");
}

Renderer2D::~Renderer2D()
{
	Gum::_delete(pGBuffer);
}


void Renderer2D::renderInternal()
{    
    if(pWorld == nullptr)
        return;

    pFramebuffer->bind();
    glClearColor(0.02, 0.31, 0.53,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pWorld->renderSky();

    for(unsigned int i = 0; i < pWorld->numLayers(); i++)
        pWorld->getSpriteLayer(i)->render();
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
}

void Renderer2D::renderIDsInternal()
{
    if(pWorld == nullptr)
        return;

    //pWorld->getObjectManager()->render(ObjectManager::WITHOUTSKYBOX, pIDRenderer->getMeshShader(), true);
    pWorld->renderRenderableIDs();
}

void Renderer2D::update()
{
    if(pWorld == nullptr)
        return;

    pWorld->update();
}

void Renderer2D::updateFramebufferSize()
{
    Renderer::updateFramebufferSize();
    pGBuffer->getFramebuffer()->setSize(pRenderCanvas->getSize());

    if(pWorld != nullptr)
        pWorld->updateProjection();
}

//Getter
G_Buffer* Renderer2D::getGBuffer() { return this->pGBuffer; }
World2D* Renderer2D::getWorld()    { return this->pWorld; }

//Setter
void Renderer2D::setWorld(World2D* world) 
{ 
    this->pWorld = world; 
    world->getLightManager()->addCallback([this](Light* light, int index) {
        //pLightning->loadLight(light, index);
    });
}


void Renderer2D::initShader()
{
    if(!Gum::ShaderManager::hasShaderProgram("ParallaxSkyShader"))
    {
        pParallaxSkyShader = new ShaderProgram();
        pParallaxSkyShader->addShader(new Shader(ParallaxSkyVertexShader, Shader::TYPES::VERTEX_SHADER));
        pParallaxSkyShader->addShader(new Shader(ParallaxSkyFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

        pParallaxSkyShader->build("SpriteShader", {{"vertices", 0}});
        pParallaxSkyShader->addUniform("color");
        pParallaxSkyShader->addTexture("textureSampler", 0);

        Gum::ShaderManager::addShaderProgram(pParallaxSkyShader, "ParallaxSkyShader");
    }
}