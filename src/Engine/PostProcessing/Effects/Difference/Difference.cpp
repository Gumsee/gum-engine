#include "Difference.h"

Difference::Difference(Canvas *canvas)
{
    init(canvas);
	this->pShader = ShaderProgram::requestShaderProgram("DifferenceShader");
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
	pRenderCanvas->render();
    pSecondTexture->unbind(1);
    texture->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();

    return pFramebuffer->getTextureAttachment(0);
}