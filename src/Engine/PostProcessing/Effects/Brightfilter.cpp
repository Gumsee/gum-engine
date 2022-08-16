#include "Brightfilter.h"
#include "../../Managers/ShaderManager.h"

Brightfilter::Brightfilter(Box* canvas)
{
	this->pShader = Gum::ShaderManager::getShaderProgram("BrightnessFilterShader");
	init(canvas);
}


Brightfilter::~Brightfilter() { }

void Brightfilter::render(Texture* texture)
{
	pFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	pShader->use();
	texture->bind(0);
	pRenderCanvas->renderCustom();
	texture->unbind(0);
	pShader->unuse();
	pFramebuffer->unbind();
}