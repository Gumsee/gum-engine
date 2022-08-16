#include "CombineFramebuffer.h"
#include "../../Managers/ShaderManager.h"


CombineFramebuffer::CombineFramebuffer(Box *canvas)
{
    init(canvas);
	pShader = Gum::ShaderManager::getShaderProgram("CombineFramebufferShader");
}

CombineFramebuffer::~CombineFramebuffer() {}

void CombineFramebuffer::render(Texture* texture)
{
	pFramebuffer->bind();
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	pShader->use();

    framebuffer1->getTextureAttachment(0)->bind(0);
    framebuffer1->getDepthTextureAttachment()->bind(1);
    framebuffer2->getTextureAttachment(0)->bind(2);
    framebuffer2->getDepthTextureAttachment()->bind(3);
	pRenderCanvas->renderCustom();
    framebuffer2->getDepthTextureAttachment()->unbind(3);
    framebuffer2->getTextureAttachment(0)->unbind(2);
    framebuffer1->getDepthTextureAttachment()->unbind(1);
    framebuffer1->getTextureAttachment(0)->unbind(0);

	pShader->unuse();
	pFramebuffer->unbind();
}

void CombineFramebuffer::addFirstFramebuffer(Framebuffer* framebuffer)
{
    this->framebuffer1 = framebuffer;
}

void CombineFramebuffer::addSecondFramebuffer(Framebuffer* framebuffer)
{
    this->framebuffer2 = framebuffer;
}