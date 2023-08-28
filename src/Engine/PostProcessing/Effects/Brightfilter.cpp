#include "Brightfilter.h"
#include "../../Shaders/ShaderManager.h"

Brightfilter::Brightfilter(Box* canvas)
{
	this->pShader = Gum::ShaderManager::getShaderProgram("BrightnessFilterShader");
	init(canvas);
}


Brightfilter::~Brightfilter() { }

void Brightfilter::render(Texture* texture)
{
	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	pShader->use();
	texture->bind(0);
	pRenderCanvas->renderCustom();
	texture->unbind(0);
	pShader->unuse();
	pFramebuffer->unbind();
}