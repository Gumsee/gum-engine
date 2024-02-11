#include "Brightfilter.h"
#include "BrightfilterShader.h"
#include "../../PostProcessing.h"

Brightfilter::Brightfilter(Box* canvas)
{
	init(canvas);
    
    if(this->pShader == nullptr)
    {
        this->pShader = new ShaderProgram("BrightnessFilterShader", true);
        this->pShader->addShader(Gum::PostProcessing::VertexShader);
        this->pShader->addShader(new Shader(BrightfilterFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        this->pShader->build();

        this->pShader->addTexture("texture0", 0);
    }
}


Brightfilter::~Brightfilter() { }

Texture* Brightfilter::render(Texture* texture)
{
	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	pShader->use();
	texture->bind(0);
	pRenderCanvas->renderCustom();
	texture->unbind(0);
	pShader->unuse();
	pFramebuffer->unbind();
    
    return pFramebuffer->getTextureAttachment(0);
}