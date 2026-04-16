#include "CombineFramebuffer.h"


CombineFramebuffer::CombineFramebuffer(Canvas *canvas)
{
  init(canvas);
	pShader = ShaderProgram::requestShaderProgram("CombineFramebufferShader");
}

CombineFramebuffer::~CombineFramebuffer() {}

Texture* CombineFramebuffer::render([[maybe_unused]] Texture* texture)
{
	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR);
	pShader->use();

  framebuffer1->getTextureAttachment(0)->bind(0);
  framebuffer1->getDepthTextureAttachment()->bind(1);
  framebuffer2->getTextureAttachment(0)->bind(2);
  framebuffer2->getDepthTextureAttachment()->bind(3);
	pRenderCanvas->render();
  framebuffer2->getDepthTextureAttachment()->unbind(3);
  framebuffer2->getTextureAttachment(0)->unbind(2);
  framebuffer1->getDepthTextureAttachment()->unbind(1);
  framebuffer1->getTextureAttachment(0)->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
    
  return pFramebuffer->getTextureAttachment(0);
}

void CombineFramebuffer::addFirstFramebuffer(Framebuffer* framebuffer)
{
    this->framebuffer1 = framebuffer;
}

void CombineFramebuffer::addSecondFramebuffer(Framebuffer* framebuffer)
{
    this->framebuffer2 = framebuffer;
}