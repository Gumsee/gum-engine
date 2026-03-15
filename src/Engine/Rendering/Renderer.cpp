#include "Renderer.h"
#include "Camera.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/Variables.h"
#include <System/MemoryManagement.h>

Renderer::Renderer(Canvas* canvas, const Type& type)
{
    iType = type;
    pRenderCanvas = canvas;

    pIDRenderer = new IDRenderer(pRenderCanvas);

    pFramebuffer = new Framebuffer(canvas->getSize());
    pFramebuffer->setClearColor(color(5, 79, 135, 255));
    pFramebuffer->addTextureAttachment<float>(0, "Renderer3DFB");
    pFramebuffer->addDepthTextureAttachment();
    //pFramebuffer->addDepthStencilTextureAttachment();

    pHighDynamicRange = new HighDynamicRange(pRenderCanvas);
    this->fExposure = 1.0f;

    if(pActiveRenderer == nullptr)
        pActiveRenderer = this;
}

Renderer::~Renderer()
{
    if(pActiveRenderer == this)
        pActiveRenderer = nullptr;
    Gum::_delete(pFramebuffer);
    Gum::_delete(pHighDynamicRange);
    Gum::_delete(pIDRenderer);
}


void Renderer::render()
{
	start = std::chrono::high_resolution_clock::now();

    renderInternal();
    
    //Apply postprocessing effects here
    Texture* lastTex = pFramebuffer->getTextureAttachment();
    for(size_t i = 0; i < vPostProcessingEffects.size(); i++)
        lastTex = vPostProcessingEffects[i]->render(lastTex);

    lastTex = pHighDynamicRange->render(lastTex, this->fExposure);

    pRenderCanvas->setTexture(lastTex);

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

void Renderer::renderIDs()
{
    pIDRenderer->bind();
	pIDRenderer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH);
    Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    
    pIDRenderer->getMeshShader()->use();
    renderIDsInternal();
}

void Renderer::makeActive()
{
    pActiveRenderer = this;
}

void Renderer::updateFramebufferSize()
{
    pFramebuffer->setSize(pRenderCanvas->getSize());
    pHighDynamicRange->setSize(pRenderCanvas->getSize());
    pIDRenderer->setSize(pRenderCanvas->getSize());
    for(size_t i = 0; i < vPostProcessingEffects.size(); i++)
        vPostProcessingEffects[i]->setSize(pRenderCanvas->getSize());
    if(Camera::getActiveCamera() != nullptr)
        Camera::getActiveCamera()->updateProjection(pRenderCanvas->getSize());
}


void Renderer::addPostProcessingEffect(PostProcessingEffect* effect) { vPostProcessingEffects.push_back(effect); }


//
// Getter
//
long long Renderer::getExecutionTime() const      { return this->microseconds; }
Canvas* Renderer::getRenderCanvas()               { return this->pRenderCanvas; }
float Renderer::getExposure() const               { return this->fExposure; }
Renderer::Type Renderer::getType() const          { return this->iType; }
Framebuffer* Renderer::getFramebuffer()           { return this->pFramebuffer; }
HighDynamicRange* Renderer::getHighDynamicRange() { return this->pHighDynamicRange; }
IDRenderer* Renderer::getIDRenderer()             { return this->pIDRenderer; }
Renderer* Renderer::getActiveRenderer()           { return pActiveRenderer; }


//
// Setter
//
void Renderer::setExposure(const float& exposure) { this->fExposure = exposure; }
void Renderer::setRenderCanvas(Canvas* canvas)    { this->pRenderCanvas = canvas; }