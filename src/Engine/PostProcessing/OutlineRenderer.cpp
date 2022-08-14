#include "OutlineRenderer.h"
#include <OpenGL/Framebuffer.h>
#include <Essentials/Window.h>
#include "OutlineRendererShader.h"
#include "../Managers/ShaderManager.h"
#include "../General/Camera.h"

OutlineRenderer::OutlineRenderer(Box* rendercanvas)
{
    initShader();
    pFramebuffer = new Framebuffer(rendercanvas->getSize());
    pFramebuffer->addTextureAttachment();
}

OutlineRenderer::~OutlineRenderer()
{

}

void OutlineRenderer::resetFramebuffer()
{
    pFramebuffer->bind();
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
    pFramebuffer->unbind();
}

void OutlineRenderer::renderObject(Instance *instance)
{
    ShaderProgram::getCurrentlyBoundShader()->LoadUniform("isInstanced", (int)false);
    ShaderProgram::getCurrentlyBoundShader()->LoadUniform("transformationMatrix", instance->getMatrix());

    glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    glStencilMask(0xFF); 
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    instance->parentObject->getVertexArrayObject()->bind();
    glDrawElements(GL_TRIANGLES, instance->parentObject->getVertexArrayObject()->numVertices(), GL_UNSIGNED_INT, 0);
    instance->parentObject->getVertexArrayObject()->unbind();
    glDisable(GL_BLEND);
}


void OutlineRenderer::renderOutlines(Instance *instance)
{
    pShader->use();
    pShader->LoadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pShader->LoadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
    pShader->LoadUniform("transformationMatrix", instance->getMatrix());

    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF); 
    //glStencilMask(0x00);
    
    //glEnable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    instance->parentObject->getVertexArrayObject()->bind();
    glDrawElements(GL_TRIANGLES, instance->parentObject->getVertexArrayObject()->numVertices(), GL_UNSIGNED_INT, 0);
    instance->parentObject->getVertexArrayObject()->unbind();
    glDisable(GL_BLEND);
    pShader->unuse();

    //glStencilMask(0xFF);
    //glStencilFunc(GL_ALWAYS, 0, 0x00); 
    glEnable(GL_DEPTH_TEST);
}

void OutlineRenderer::initShader()
{
    if(Gum::ShaderManager::hasShaderProgram("OutlineRendererVertexShader"))
    {
		pShader = Gum::ShaderManager::getShaderProgram("OutlineRendererVertexShader");
    }
    else
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(OutlineRendererVertexShader, Shader::VERTEX_SHADER));
        pShader->addShader(new Shader(OutlineRendererFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("OutlineRendererVertexShader");



        Gum::ShaderManager::addShaderProgram(pShader);
    }
}

Framebuffer* OutlineRenderer::getFramebuffer()
{
    return this->pFramebuffer;
}