#include "CombineTextures.h"
#include "../../Managers/ShaderManager.h"


CombineTextures::CombineTextures(Box *canvas, ivec2 resolution)
{
    init(canvas, resolution);
	pShader = Gum::ShaderManager::getShaderProgram("CombineTexturesShader");
}

CombineTextures::~CombineTexturess() {}

void CombineTextures::render(Texture* texture1, Texture* texture2, float exposure)
{
	pFramebuffer->bind();

	pShader->use();
	pShader->LoadUniform("exposure", exposure);

    texture1->bind(0);
    texture2->bind(1);
	pRenderCanvas->render();
    texture2->unbind(1);
    texture1->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
}