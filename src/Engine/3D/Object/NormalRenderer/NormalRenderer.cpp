#include "NormalRenderer.h"
#include "NormalRendererShader.h"

NormalRenderer::NormalRenderer(World3D* world, const float& normallength)
    : fNormalLength(normallength)
{
    this->pWorld = world;

    if(pShader == nullptr)
    {
        pShader = new ShaderProgram(true);
        pShader->addShader(new Shader(NormalRendererVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(NormalRendererGeometryShader, Shader::TYPES::GEOMETRY_SHADER));
        pShader->addShader(new Shader(NormalRendererFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("NormalRendererShader", { {"vertices", 0}, {"normals", 2}, {"transMatrices", 3} });

        pShader->addUniform("length");
    }
}

NormalRenderer::~NormalRenderer()
{

}


void NormalRenderer::render()
{
    pShader->use();
    pShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pShader->loadUniform("length", fNormalLength);
    pWorld->getObjectManager()->renderEverything();
    pShader->unuse();
}