#include "IDRenderer.h"
#include "IDRendererShader.h"

IDRenderer::IDRenderer(Canvas* canvas)
    : Framebuffer(canvas->getSize(), false)
{
  pCanvas = canvas;

  addTextureAttachment<unsigned char>(0, "IndividualMap", 3);
  addDepthAttachment();

  pMeshIDShader = ShaderProgram::requestShaderProgram("MeshIDShader", true);
  pMeshIDShader->addShader(new Shader(MeshIDVertexShader, Shader::TYPES::VERTEX_SHADER));
  pMeshIDShader->addShader(new Shader(MeshIDFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pMeshIDShader->build({ {"vertices", 0}, {"TransMatrix", 3}, {"individualColor", 10} });
}

IDRenderer::~IDRenderer()
{
}

ShaderProgram* IDRenderer::getMeshShader()
{
  return pMeshIDShader;
}

Texture2D* IDRenderer::getResultTexture()
{
  return (Texture2D*)getTextureAttachment();
}