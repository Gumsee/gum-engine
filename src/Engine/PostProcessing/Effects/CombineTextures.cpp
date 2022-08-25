#include "CombineTextures.h"
#include "CombineTexturesShader.h"
#include "../../Managers/ShaderManager.h"
#include <Essentials/MemoryManagement.h>

ShaderProgram* CombineTextures::pShader = nullptr;

CombineTextures::CombineTextures(Box *canvas)
{
    init(canvas);
	initShader();
}

CombineTextures::~CombineTextures() 
{
    Gum::_delete(pShader);
}

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


void CombineTextures::initShader()
{
	if(pShader == nullptr)
	{
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(CombineTexturesVertexShader, Shader::VERTEX_SHADER));
        pShader->addShader(new Shader(CombineTexturesFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("CombineTexturesShader");
        pShader->addUniform("exposure");
        pShader->addTexture("texture1", 0);
        pShader->addTexture("texture2", 1);
	}
}