#include "ContrastChanger.h"
#include "../../Managers/ShaderManager.h"


ContrastChanger::ContrastChanger(Box *canvas)
{
    init(canvas);
	this->pShader = Gum::ShaderManager::getShaderProgram("ContrastChangerShader");
	pShader->addTexture("Texture1", 0);
}


ContrastChanger::~ContrastChanger() {}

void ContrastChanger::render(Texture* texture, int contrast)
{
	pFramebuffer->bind();
	pShader->use();
	pShader->loadUniform("Contrast", contrast);
	texture->bind();
	pRenderCanvas->renderCustom();
	texture->unbind();

	pShader->unuse();
	pFramebuffer->unbind();
}