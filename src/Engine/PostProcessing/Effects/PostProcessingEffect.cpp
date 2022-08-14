#include "PostProcessingEffect.h"

PostProcessingEffect::PostProcessingEffect() {}
PostProcessingEffect::~PostProcessingEffect() {}
void PostProcessingEffect::init(Box* canvas, ivec2 resolution)
{
    this->pRenderCanvas = canvas;
	this->pFramebuffer = new Framebuffer(resolution);
    this->pFramebuffer->addTextureAttachment(0, "PostProcessingEffect", GL_RGBA, GL_RGBA);
}

void PostProcessingEffect::render(Texture* texture) {}

Texture* PostProcessingEffect::getResultTexture() { return pFramebuffer->getTextureAttachment(0); }