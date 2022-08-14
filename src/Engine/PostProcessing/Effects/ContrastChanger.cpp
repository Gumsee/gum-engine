#include "ContrastChanger.h"
#include "../../Managers/ShaderManager.h"


ContrastChanger::ContrastChanger(Box *canvas, ivec2 resolution)
{
    init(canvas, resolution);
	this->pShader = Gum::ShaderManager::getShaderProgram("ContrastChangerShader");
	pShader->addTexture("Texture1", 0);
}


ContrastChanger::~ContrastChanger() {}

void ContrastChanger::render(Texture* texture, int contrast)
{
	pFramebuffer->bind();
	pShader->use();
	pShader->LoadUniform("Contrast", contrast);
	texture->bind();
	pRenderCanvas->render();
	texture->unbind();

	pShader->unuse();
	pFramebuffer->unbind();
}