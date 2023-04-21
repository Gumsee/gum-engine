#include "Difference.h"
#include "../../Managers/ShaderManager.h"

Difference::Difference(Box *canvas)
{
    init(canvas);
	this->pShader = Gum::ShaderManager::getShaderProgram("DifferenceShader");
}


Difference::~Difference(){ }

void Difference::render(Texture* texture1, Texture* texture2)
{
	pFramebuffer->bind();
	pShader->use();
	pShader->loadUniform("Texture1", 0);
	pShader->loadUniform("Texture2", 1);

    texture1->bind(0);
    texture2->bind(1);
	pRenderCanvas->renderCustom();
    texture2->unbind(1);
    texture1->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
}