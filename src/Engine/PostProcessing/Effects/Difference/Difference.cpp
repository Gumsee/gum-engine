#include "Difference.h"
#include "../../../Shaders/ShaderManager.h"

Difference::Difference(Box *canvas)
{
    init(canvas);
	this->pShader = Gum::ShaderManager::getShaderProgram("DifferenceShader");
}


Difference::~Difference(){ }

Texture* Difference::render(Texture* texture)
{
	pFramebuffer->bind();
	pShader->use();
	pShader->loadUniform("Texture1", 0);
	pShader->loadUniform("Texture2", 1);

    texture->bind(0);
    pSecondTexture->bind(1);
	pRenderCanvas->renderCustom();
    pSecondTexture->unbind(1);
    texture->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();

    return pFramebuffer->getTextureAttachment(0);
}