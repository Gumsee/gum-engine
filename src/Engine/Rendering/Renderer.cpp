#include "Renderer.h"
#include "Camera.h"
#include <System/MemoryManagement.h>

Renderer::Renderer(Box* canvas)
{
    pRenderCanvas = canvas;
    pRenderCanvas->invertTexcoordY(true);

    pIDRenderer = new IDRenderer(pRenderCanvas);

    pFramebuffer = new Framebuffer(canvas->getSize());
    pFramebuffer->addTextureAttachment(0, "Renderer3DFB", GL_RGBA, GL_RGBA16F);
    pFramebuffer->addDepthTextureAttachment();
    //pFramebuffer->addDepthStencilTextureAttachment();

    pOcclusionMask = new OcclusionMask(canvas->getSize().x, canvas->getSize().y);
    pHighDynamicRange = new HighDynamicRange(pRenderCanvas);
    this->fExposure = 1.0f;

    if(pActiveRenderer == nullptr)
        pActiveRenderer = this;
}

Renderer::~Renderer()
{
    Gum::_delete(pOcclusionMask);
    Gum::_delete(pFramebuffer);
    Gum::_delete(pHighDynamicRange);
}


void Renderer::render()
{
	start = std::chrono::high_resolution_clock::now();

    //Render Occlusion Mask via CPU
    pOcclusionMask->render();

    renderInternal();
    
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

    pRenderCanvas->setTexture(lastTex);

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

void Renderer::renderIDs()
{
    pIDRenderer->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    pIDRenderer->getMeshShader()->use();
    pIDRenderer->getMeshShader()->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pIDRenderer->getMeshShader()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    renderIDsInternal();
}

void Renderer::makeActive()
{
    pActiveRenderer = this;
}

void Renderer::updateFramebufferSize()
{
    pFramebuffer->setSize(pRenderCanvas->getSize());
    pIDRenderer->setSize(pRenderCanvas->getSize());
}


void Renderer::addPostProcessingEffect(PostProcessingEffect* effect) { vPostProcessingEffects.push_back(effect); }


//
// Getter
//
long long Renderer::getExecutionTime() const      { return this->microseconds; }
Box* Renderer::getRenderCanvas()                  { return this->pRenderCanvas; }
float Renderer::getExposure() const               { return this->fExposure; }
Framebuffer* Renderer::getFramebuffer()           { return this->pFramebuffer; }
IDRenderer* Renderer::getIDRenderer()             { return this->pIDRenderer; }
Renderer* Renderer::getActiveRenderer()           { return pActiveRenderer; }


//
// Setter
//
void Renderer::setExposure(const float& exposure) { this->fExposure = exposure; }
void Renderer::setRenderCanvas(Box* canvas)       { this->pRenderCanvas = canvas; }