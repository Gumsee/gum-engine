#include "CombineTextures.h"
#include "CombineTexturesShader.h"
#include "../../PostProcessing.h"

CombineTextures::CombineTextures(Box *canvas, const float& exposure)
{
    init(canvas);
    this->fExposure = exposure;
    pSecondTexture = nullptr;

	if(pShader == nullptr)
	{
        pShader = new ShaderProgram("CombineTexturesShader", true);
        pShader->addShader(Gum::PostProcessing::VertexShader);
        pShader->addShader(new Shader(CombineTexturesFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build();
        pShader->addUniform("exposure");
        pShader->addTexture("texture0", 0);
        pShader->addTexture("texture1", 1);
	}
}

CombineTextures::~CombineTextures() 
{
}

Texture* CombineTextures::render(Texture* texture)
{
    if(pSecondTexture == nullptr)
        return texture;

    pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR);

	pShader->use();
	pShader->loadUniform("exposure", fExposure);

    texture->bind(0);
    pSecondTexture->bind(1);
	pRenderCanvas->renderCustom();
    pSecondTexture->unbind(1);
    texture->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
    
    return pFramebuffer->getTextureAttachment(0);
}