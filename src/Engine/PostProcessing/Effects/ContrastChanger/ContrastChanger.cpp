#include "ContrastChanger.h"
#include "ContrastChangerShader.h"
#include "../../PostProcessing.h"


ContrastChanger::ContrastChanger(Canvas *canvas, const float& contrast)
{
  init(canvas);
  this->fContrast = contrast;

  this->pShader = ShaderProgram::requestShaderProgram("ContrastChangerShader", true);
  this->pShader->addShader(Gum::PostProcessing::VertexShader);
  this->pShader->addShader(new Shader(ContrastChangerFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  this->pShader->build();

  this->pShader->addTexture("texture0", 0);
}


ContrastChanger::~ContrastChanger()
{

}

Texture* ContrastChanger::render(Texture* texture)
{
	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	pShader->use();
	pShader->loadUniform("contrast", fContrast);
	texture->bind();
	pRenderCanvas->render();
	texture->unbind();

	pShader->unuse();
	pFramebuffer->unbind();
    
  return pFramebuffer->getTextureAttachment(0);
}