#include "CombineTextures.h"
#include "CombineTexturesShader.h"
#include "../../Shaders/ShaderManager.h"
#include <System/MemoryManagement.h>

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
	pShader->loadUniform("exposure", exposure);

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
        pShader = new ShaderProgram(true);
        pShader->addShader(new Shader(CombineTexturesVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(CombineTexturesFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("CombineTexturesShader");
        pShader->addUniform("exposure");
        pShader->addTexture("texture1", 0);
        pShader->addTexture("texture2", 1);
	}
}