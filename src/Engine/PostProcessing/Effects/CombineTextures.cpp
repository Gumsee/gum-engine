#include "CombineTextures.h"
#include "../../Managers/ShaderManager.h"


CombineTextures::CombineTextures(Box *canvas)
{
    init(canvas);
	pShader = Gum::ShaderManager::getShaderProgram("CombineTexturesShader");
}

CombineTextures::~CombineTextures() {}

void CombineTextures::render(Texture* texture1, Texture* texture2, float exposure)
{
	pFramebuffer->bind();

	pShader->use();
	pShader->LoadUniform("exposure", exposure);

    texture1->bind(0);
    texture2->bind(1);
	pRenderCanvas->renderCustom();
    texture2->unbind(1);
    texture1->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
}