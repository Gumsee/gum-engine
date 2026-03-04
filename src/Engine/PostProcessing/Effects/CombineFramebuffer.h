#pragma once
#include "PostProcessingEffect.h"

class CombineFramebuffer : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;
    Framebuffer* framebuffer1;
    Framebuffer* framebuffer2;

public:
	CombineFramebuffer(Canvas *canvas);
	~CombineFramebuffer();
	Texture* render(Texture* texture = nullptr);

    void addFirstFramebuffer(Framebuffer* framebuffer);
    void addSecondFramebuffer(Framebuffer* framebuffer);
};