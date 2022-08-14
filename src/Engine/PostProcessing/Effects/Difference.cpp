#include "Difference.h"
#include "../../Managers/ShaderManager.h"

Difference::Difference(Box *canvas, ivec2 resolution)
{
    init(canvas, resolution);
	this->pShader = Gum::ShaderManager::getShaderProgram("DifferenceShader");
}


Difference::~Difference(){ }

void Difference::render(Texture* texture1, Texture* texture2)
{
	pFramebuffer->bind();
	pShader->use();
	pShader->LoadUniform("Texture1", 0);
	pShader->LoadUniform("Texture2", 1);

    texture1->bind(0);
    texture2->bind(1);
	pRenderCanvas->render();
    texture2->unbind(1);
    texture1->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
}