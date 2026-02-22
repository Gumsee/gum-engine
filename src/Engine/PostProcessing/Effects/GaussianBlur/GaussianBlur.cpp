#include "GaussianBlur.h"
#include "Graphics/Framebuffer.h"
#include <System/MemoryManagement.h>
#include "GaussianBlurShader.h"
#include "../../PostProcessing.h"
#include "Graphics/Shader.h"


GaussianBlur::GaussianBlur(Box *canvas, int stage)
{
	init(canvas);

	if (stage % 2 == 0)
	{
		BlurryDivider1 = stage / 2;
		BlurryDivider2 = stage;
	}
    Gum::_delete(pFramebuffer);
    
    pBlurFramebufferH = new Framebuffer(canvas->getSize() / BlurryDivider1);
    pFramebuffer = new Framebuffer(canvas->getSize() / BlurryDivider2);
    pBlurFramebufferV = new Framebuffer(canvas->getSize() / BlurryDivider1);
    pBlurFramebufferV2 = new Framebuffer(canvas->getSize() / BlurryDivider2);

    pBlurFramebufferH->addTextureAttachment<unsigned char>(0)->setFiltering(Texture::LINEAR);
    pFramebuffer->addTextureAttachment<unsigned char>(0)->setFiltering(Texture::LINEAR);
    pBlurFramebufferV->addTextureAttachment<unsigned char>(0)->setFiltering(Texture::LINEAR);
    pBlurFramebufferV2->addTextureAttachment<unsigned char>(0)->setFiltering(Texture::LINEAR);


    if(VblurShader == nullptr)
    {
        VblurShader = new ShaderProgram("GaussianBlurVShader", true);
        VblurShader->addShader(new Shader(GussianBlurVShaderVert, Shader::TYPES::VERTEX_SHADER));
        VblurShader->addShader(new Shader(GussianBlurShaderFrag, Shader::TYPES::FRAGMENT_SHADER));
        VblurShader->build();

        VblurShader->addTexture("textureSampler", 0);
    }

    if(HblurShader == nullptr)
    {
        HblurShader = new ShaderProgram("GaussianBlurHShader", true);
        HblurShader->addShader(new Shader(GussianBlurHShaderVert, Shader::TYPES::VERTEX_SHADER));
        HblurShader->addShader(new Shader(GussianBlurShaderFrag, Shader::TYPES::FRAGMENT_SHADER));
        HblurShader->build();

        HblurShader->addTexture("textureSampler", 0);
    }
}


GaussianBlur::~GaussianBlur() 
{
	//Gum::_delete(HblurShader);
	//Gum::_delete(VblurShader);
}


Texture* GaussianBlur::render(Texture* RenderResult)
{
	pBlurFramebufferV->bind();
	pBlurFramebufferV->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	VblurShader->use();
	VblurShader->loadUniform("specialVar", (float)pBlurFramebufferV->getSize().x);
	RenderResult->bind();
	pRenderCanvas->renderCustom();
	RenderResult->unbind();
	VblurShader->unuse();

	pBlurFramebufferH->bind();
	pBlurFramebufferH->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	HblurShader->use();
	HblurShader->loadUniform("specialVar", (float)pBlurFramebufferH->getSize().y);
	pBlurFramebufferV->getTextureAttachment(0)->bind();
	pRenderCanvas->renderCustom();
	pBlurFramebufferV->getTextureAttachment(0)->unbind();
	HblurShader->unuse();

	pBlurFramebufferV2->bind();
	pBlurFramebufferV2->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	VblurShader->use();
	VblurShader->loadUniform("specialVar", (float)pBlurFramebufferV2->getSize().x);
	pBlurFramebufferH->getTextureAttachment(0)->bind();
	pRenderCanvas->renderCustom();
	pBlurFramebufferH->getTextureAttachment(0)->unbind();
	VblurShader->unuse();

	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::STENCIL);
	HblurShader->use();
	HblurShader->loadUniform("specialVar", (float)pFramebuffer->getSize().y);
	pBlurFramebufferV2->getTextureAttachment(0)->bind();
	pRenderCanvas->renderCustom();
	pBlurFramebufferV2->getTextureAttachment(0)->unbind();
	HblurShader->unuse();
	pFramebuffer->unbind();

    return pFramebuffer->getTextureAttachment(0);
}