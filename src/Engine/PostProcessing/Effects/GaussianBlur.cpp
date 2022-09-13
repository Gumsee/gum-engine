#include "GaussianBlur.h"
#include "../../Managers/ShaderManager.h"
#include <System/MemoryManagement.h>


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

    pBlurFramebufferH->addTextureAttachment(0);
    pFramebuffer->addTextureAttachment(0);
    pBlurFramebufferV->addTextureAttachment(0);
    pBlurFramebufferV2->addTextureAttachment(0);

    
    /*pBlurFramebufferH->addDepthAttachment();
    pBlurFramebufferH2->addDepthAttachment();
    pBlurFramebufferV->addDepthAttachment();
    pBlurFramebufferV2->addDepthAttachment();*/


	VblurShader = Gum::ShaderManager::getShaderProgram("GaussianBlurVShader");
	HblurShader = Gum::ShaderManager::getShaderProgram("GaussianBlurHShader");
}


GaussianBlur::~GaussianBlur() 
{
	Gum::_delete(HblurShader);
	Gum::_delete(VblurShader);
}


void GaussianBlur::render(Texture* RenderResult)
{
	pBlurFramebufferV->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	VblurShader->use();
	VblurShader->LoadUniform("specialVar", (float)pBlurFramebufferV->getSize().x);
	RenderResult->bind();
	pRenderCanvas->renderCustom();
	RenderResult->unbind();
	VblurShader->unuse();

	pBlurFramebufferH->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	HblurShader->use();
	HblurShader->LoadUniform("specialVar", (float)pBlurFramebufferH->getSize().y);
	pBlurFramebufferV->getTextureAttachment(0)->bind();
	pRenderCanvas->renderCustom();
	pBlurFramebufferV->getTextureAttachment(0)->unbind();
	HblurShader->unuse();

	pBlurFramebufferV2->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	VblurShader->use();
	VblurShader->LoadUniform("specialVar", (float)pBlurFramebufferV2->getSize().x);
	pBlurFramebufferH->getTextureAttachment(0)->bind();
	pRenderCanvas->renderCustom();
	pBlurFramebufferH->getTextureAttachment(0)->unbind();
	VblurShader->unuse();

	pFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	HblurShader->use();
	HblurShader->LoadUniform("specialVar", (float)pFramebuffer->getSize().y);
	pBlurFramebufferV2->getTextureAttachment(0)->bind();
	pRenderCanvas->renderCustom();
	pBlurFramebufferV2->getTextureAttachment(0)->unbind();
	HblurShader->unuse();
	pFramebuffer->unbind();
}