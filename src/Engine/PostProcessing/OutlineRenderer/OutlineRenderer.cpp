#include "OutlineRenderer.h"
#include <Graphics/Framebuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/Variables.h>
#include <System/MemoryManagement.h>

#include "OutlineRendererShader.h"
#include "../PostProcessing.h"
#include "../../Rendering/Camera.h"

OutlineRenderer::OutlineRenderer(Canvas* rendercanvas)
{
  pShader = nullptr;
  pCanvas = rendercanvas;
  initShader();
  setColor(rgba(92, 97, 163, 255));

  pFramebuffer = new Framebuffer(rendercanvas->getSize());
  pFramebuffer->addTextureAttachment<unsigned char>(0, "Outlines", 1);
}

OutlineRenderer::~OutlineRenderer()
{
}


void OutlineRenderer::render(Renderable* renderable, mat4 transform)
{
  Framebuffer* current = Framebuffer::CurrentlyBoundFramebuffer;
  pFramebuffer->bind();
  pFramebuffer->clear(Framebuffer::ClearFlags::COLOR);
  Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
  pShader->use();
  pShader->loadUniform("transformationMatrix", transform);

  renderable->render();

  current->bind();
  pProcessingShader->use();
  pFramebuffer->getTextureAttachment(0)->bind(0);
  pCanvas->render();
  pFramebuffer->getTextureAttachment(0)->unbind(0);

  Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
}

void OutlineRenderer::initShader()
{
  pShader = ShaderProgram::requestShaderProgram("OutlineRendererShader", true);
  pShader->addShader(new Shader(OutlineRendererVertexShader, Shader::TYPES::VERTEX_SHADER));
  pShader->addShader(new Shader(OutlineRendererFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pShader->build();

  pProcessingShader = ShaderProgram::requestShaderProgram("OutlineRendererProcessingShader", true);
  pProcessingShader->addShader(Gum::PostProcessing::VertexShader);
  pProcessingShader->addShader(new Shader(OutlineRendererProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pProcessingShader->build();
}

void OutlineRenderer::updateSize()
{
  pFramebuffer->setSize(pCanvas->getSize());
}


void OutlineRenderer::setColor(const color& col)
{
  this->cColor = col;
  ShaderProgram* currentShader = ShaderProgram::getCurrentlyBoundShader();
  pProcessingShader->use();
  pProcessingShader->loadUniform("color", cColor);
  
  if(currentShader != nullptr)
    currentShader->use();
}

color OutlineRenderer::getColor() const
{
  return cColor;
}