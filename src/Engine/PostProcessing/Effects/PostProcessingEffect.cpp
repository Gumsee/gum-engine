#include "PostProcessingEffect.h"

PostProcessingEffect::PostProcessingEffect() {}
PostProcessingEffect::~PostProcessingEffect() {}
void PostProcessingEffect::init(Box* canvas)
{
    this->pRenderCanvas = canvas;
	this->pFramebuffer = new Framebuffer(pRenderCanvas->getSize());
    this->pFramebuffer->addTextureAttachment(0, "PostProcessingEffect", GL_RGBA, GL_RGBA);
}

void PostProcessingEffect::render(Texture* texture) {}

Texture* PostProcessingEffect::getResultTexture() { return pFramebuffer->getTextureAttachment(0); }