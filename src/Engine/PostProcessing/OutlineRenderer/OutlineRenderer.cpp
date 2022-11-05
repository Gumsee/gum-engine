#include "OutlineRenderer.h"
#include <OpenGL/Framebuffer.h>
#include <OS/Window.h>
#include <System/MemoryManagement.h>

#include "OutlineRendererShader.h"
#include "../../Managers/ShaderManager.h"
#include "../../General/Camera.h"

OutlineRenderer::OutlineRenderer(Box* rendercanvas)
{
    pShader = nullptr;
    pProcessingShader = nullptr;
    pCanvas = rendercanvas;
    initShader();
    pFramebuffer = new Framebuffer(rendercanvas->getSize());
    pFramebuffer->addTextureAttachment();
    pFramebuffer->addDepthAttachment();

    pProcessingFramebuffer = new Framebuffer(rendercanvas->getSize());
    pProcessingFramebuffer->addTextureAttachment();
    pProcessingFramebuffer->addDepthAttachment();
}

OutlineRenderer::~OutlineRenderer()
{
    Gum::_delete(pFramebuffer);
    Gum::_delete(pProcessingFramebuffer);
    Gum::_delete(pShader);
    Gum::_delete(pProcessingShader);
}


void OutlineRenderer::render(Instance *instance, Framebuffer* fb)
{
    pFramebuffer->bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pShader->use();
    pShader->LoadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pShader->LoadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
    pShader->LoadUniform("transformationMatrix", instance->getMatrix());

    instance->parentObject->getVertexArrayObject()->bind();
    glDrawElements(GL_TRIANGLES, instance->parentObject->getVertexArrayObject()->numVertices(), GL_UNSIGNED_INT, 0);
    instance->parentObject->getVertexArrayObject()->unbind();
    pShader->unuse();

    glDisable(GL_DEPTH_TEST);
    pFramebuffer->unbind();

    fb->blitDepthToOtherFramebuffer(pFramebuffer);
    pFramebuffer->blitDepthToOtherFramebuffer(pProcessingFramebuffer);


    pProcessingFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
    pProcessingShader->use();
    pFramebuffer->getTextureAttachment()->bind();
    pCanvas->renderCustom();
    pFramebuffer->getTextureAttachment()->unbind();
    pProcessingShader->unuse();
    pProcessingFramebuffer->unbind();
}

void OutlineRenderer::initShader()
{
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(OutlineRendererVertexShader, Shader::VERTEX_SHADER));
        pShader->addShader(new Shader(OutlineRendererFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("OutlineRendererShader");
    }
    if(pProcessingShader == nullptr)
    {
        pProcessingShader = new ShaderProgram();
        pProcessingShader->addShader(new Shader(OutlineRendererProcessingVertexShader, Shader::VERTEX_SHADER));
        pProcessingShader->addShader(new Shader(OutlineRendererProcessingFragmentShader, Shader::FRAGMENT_SHADER));
        pProcessingShader->build("OutlineRendererProcessingShader");
        pProcessingShader->addUniform("resolution");
        pProcessingShader->addTexture("textureSampler", 0);
    }
}

Framebuffer* OutlineRenderer::getFramebuffer()
{
    return this->pFramebuffer;
}

Texture2D* OutlineRenderer::getResultTexture()
{
    return (Texture2D*)this->pProcessingFramebuffer->getTextureAttachment();
}