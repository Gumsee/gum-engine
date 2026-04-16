#include "G_Buffer.h"
#include "GBufferShader.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Graphics.h"
#include <System/MemoryManagement.h>
#include <Graphics/Material.h>

/*
    0 PositionMap;
    1 IndividualColorMap
    2 NormalMap;
    3 DiffuseMap;
    4 ObjectDataMap;
*/
G_Buffer::G_Buffer(const ivec2& canvassize)
{
  gBuffer = new Framebuffer(canvassize);
  gBuffer->addTextureAttachment<float>(0, "G_BufferPositionMap");
  gBuffer->addTextureAttachment<float>(1, "G_BufferNormalMap");
  gBuffer->addTextureAttachment<unsigned char>(2, "G_BufferDiffuseMap");
  gBuffer->addTextureAttachment<unsigned char>(3, "G_BufferObjectDataMap");

  //gBuffer->addDepthAttachment();
  gBuffer->addDepthTextureAttachment();
  //gBuffer->addDepthStencilTextureAttachment("GBufferDepthTextureAttachment");

  pShader = nullptr;
  initShader();
}

G_Buffer::~G_Buffer()
{
  Gum::_delete(gBuffer);
}

void G_Buffer::bind()
{
  clock.reset();
  gBuffer->bind();
  gBuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH);
  //glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_COLOR);  
  //glDepthFunc(GL_LESS);
  Gum::Graphics::disableFeature(Gum::Graphics::Features::STENCIL_TESTING);
  //glDepthMask(GL_FALSE);
}

void G_Buffer::unbind()
{
  //glDisable(GL_BLEND);
  gBuffer->unbind();
}


//
// Getter
//
int G_Buffer::getDepthBuffer()              { return this->gBuffer->getDepthAttachmentID(); }
ShaderProgram *G_Buffer::getShader()        { return this->pShader; }
Texture* G_Buffer::getPositionMap()         { return this->gBuffer->getTextureAttachment(0); }
Texture* G_Buffer::getNormalMap()           { return this->gBuffer->getTextureAttachment(1); }
Texture* G_Buffer::getDiffuseMap()          { return this->gBuffer->getTextureAttachment(2); }
Texture* G_Buffer::getObjectDataMap()       { return this->gBuffer->getTextureAttachment(3); }
Texture* G_Buffer::getDepthMap()            { return this->gBuffer->getDepthTextureAttachment(); }
long long G_Buffer::getExecutionTime()      { return this->clock.getPassedTimeInMillis(); }
Framebuffer* G_Buffer::getFramebuffer()     { return this->gBuffer; }


void G_Buffer::initShader()
{
  pShader = ShaderProgram::requestShaderProgram("GBufferShader", true);
  pShader->addShader(new Shader(GBufferVertexShader, Shader::TYPES::VERTEX_SHADER));
  pShader->addShader(new Shader(GBufferFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pShader->build();

  pShader->addTexture("texture0", GUM_MATERIAL_TEXTURE0);
  pShader->addTexture("ambientOcclusionmap", GUM_MATERIAL_AMBIENT_OCCLUSION_MAP);
  pShader->addTexture("roughnessmap", GUM_MATERIAL_ROUGHNESS_MAP);
  pShader->addTexture("specularmap", GUM_MATERIAL_SPECULAR_MAP);
  pShader->addTexture("refractionmap", GUM_MATERIAL_REFRACTION_MAP);
  pShader->addTexture("reflectionmap", GUM_MATERIAL_REFLECTION_MAP);
  pShader->addTexture("Displacement", GUM_MATERIAL_DISPLACEMENT_MAP);
  pShader->addTexture("normalmap", GUM_MATERIAL_NORMAL_MAP);
  pShader->addTexture("Enviorment", 15);
  pShader->addTexture("ShadowMap", 16);
}