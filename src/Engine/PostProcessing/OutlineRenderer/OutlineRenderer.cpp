#include "OutlineRenderer.h"
#include <Graphics/Framebuffer.h>
#include <Desktop/Window.h>
#include <System/MemoryManagement.h>

#include "OutlineRendererShader.h"
#include "../../Shaders/ShaderManager.h"
#include "../../Rendering/Camera.h"

OutlineRenderer::OutlineRenderer(Box* rendercanvas)
{
    pShader = nullptr;
    pCanvas = rendercanvas;
    initShader();
}

OutlineRenderer::~OutlineRenderer()
{
    Gum::_delete(pShader);
}


void OutlineRenderer::render(Renderable* renderable, mat4 transform)
{
    glDisable(GL_DEPTH_TEST);
    pShader->use();
    pShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pShader->loadUniform("transformationMatrix", transform);

    renderable->render();
    glEnable(GL_DEPTH_TEST);
}

void OutlineRenderer::initShader()
{
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(OutlineRendererVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(OutlineRendererFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("OutlineRendererShader");
    }
}