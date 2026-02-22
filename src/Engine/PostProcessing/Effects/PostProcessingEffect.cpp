#include "PostProcessingEffect.h"
#include "Graphics/Variables.h"
#include <System/MemoryManagement.h>

PostProcessingEffect::PostProcessingEffect() 
{
    this->pFramebuffer = nullptr;
}

PostProcessingEffect::~PostProcessingEffect() 
{
    Gum::_delete(pFramebuffer);
}

void PostProcessingEffect::init(Box* canvas)
{
    this->pRenderCanvas = canvas;
	this->pFramebuffer = new Framebuffer(pRenderCanvas->getSize());
    this->pFramebuffer->addTextureAttachment<float>(0, "PostProcessingEffect");
}

Texture* PostProcessingEffect::render(Texture* texture) { return texture; }

void PostProcessingEffect::setSize(ivec2 size)
{
    this->pFramebuffer->setSize(size);
}