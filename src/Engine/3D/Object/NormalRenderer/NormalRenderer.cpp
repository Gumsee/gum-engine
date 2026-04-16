#include "NormalRenderer.h"
#include "NormalRendererShader.h"

NormalRenderer::NormalRenderer(World3D* world, const float& normallength)
    : fNormalLength(normallength)
{
  this->pWorld = world;

  pShader = ShaderProgram::requestShaderProgram("NormalRendererShader", true);
  pShader->addShader(new Shader(NormalRendererVertexShader, Shader::TYPES::VERTEX_SHADER));
  pShader->addShader(new Shader(NormalRendererGeometryShader, Shader::TYPES::GEOMETRY_SHADER));
  pShader->addShader(new Shader(NormalRendererFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pShader->build({ {"vertices", 0}, {"normals", 2}, {"transMatrices", 3} });
}

NormalRenderer::~NormalRenderer()
{

}


void NormalRenderer::render()
{
  pShader->use();
  pShader->loadUniform("length", fNormalLength);
  pWorld->getObjectManager()->renderEverything();
  pShader->unuse();
}