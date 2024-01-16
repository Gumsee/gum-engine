#include "ContrastChanger.h"
#include "ContrastChangerShader.h"
#include "../../../Shaders/ShaderManager.h"
#include "../../PostProcessing.h"


ContrastChanger::ContrastChanger(Box *canvas, const float& contrast)
{
    init(canvas);
    this->fContrast = contrast;

    if(this->pShader == nullptr)
    {
        this->pShader = new ShaderProgram(true);
        this->pShader->addShader(Gum::ShaderManager::getShader(Gum::PostProcessing::iVertexShaderID));
        this->pShader->addShader(new Shader(ContrastChangerFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        this->pShader->build("ContrastChangerShader");

        this->pShader->addTexture("texture0", 0);
	    this->pShader->addUniform("contrast");
    }
}


ContrastChanger::~ContrastChanger()
{

}

Texture* ContrastChanger::render(Texture* texture)
{
	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	pShader->use();
	pShader->loadUniform("contrast", fContrast);
	texture->bind();
	pRenderCanvas->renderCustom();
	texture->unbind();

	pShader->unuse();
	pFramebuffer->unbind();
    
    return pFramebuffer->getTextureAttachment(0);
}