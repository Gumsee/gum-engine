#include "Renderer2D.h"
#include "System/Output.h"
#include "World2D.h"
#include "../Rendering/Camera.h"
#include "../Shaders/ParallaxSkyShader.h"

#include <System/MemoryManagement.h>

Renderer2D::Renderer2D(Canvas* canvas)
    : Renderer(canvas, RENDERER2D)
{
    pWorld = nullptr;
	pGBuffer = new G_Buffer(pRenderCanvas->getSize());

    initShader();

    pParticleShader = ShaderProgram::getShaderProgramByName("ParticleShader");
    pBillboardShader = ShaderProgram::getShaderProgramByName("BillboardShader");
}

Renderer2D::~Renderer2D()
{
	Gum::_delete(pGBuffer);
}


void Renderer2D::renderInternal()
{    
    if(pWorld == nullptr)
        return;

    ShaderProgram::loadUniformForAll("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    ShaderProgram::loadUniformForAll("canvassize", pRenderCanvas->getSize());
    pFramebuffer->bind();
    pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH | Framebuffer::ClearFlags::STENCIL);
    pWorld->renderSky();

    for(unsigned int i = 0; i < pWorld->numLayers(); i++)
        pWorld->getSpriteLayer(i)->render();
    pWorld->renderRenderable();

    pParticleShader->use();
    pWorld->renderParticles(pParticleShader);

	pBillboardShader->use();
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
    //world->getLightManager()->addCallback([](Light* light, int index) {
    //    //pLightning->loadLight(light, index);
    //});
}


void Renderer2D::initShader()
{
    if(pParallaxSkyShader == nullptr)
    {
        pParallaxSkyShader = new ShaderProgram("ParallaxSkyShader", true);
        pParallaxSkyShader->addShader(new Shader(ParallaxSkyVertexShader, Shader::TYPES::VERTEX_SHADER));
        pParallaxSkyShader->addShader(new Shader(ParallaxSkyFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

        pParallaxSkyShader->build({{"vertices", 0}});
        pParallaxSkyShader->addTexture("textureSampler", 0);
    }
}