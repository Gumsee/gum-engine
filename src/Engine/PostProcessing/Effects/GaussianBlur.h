#pragma once
#include "PostProcessingEffect.h"

class GaussianBlur : public PostProcessingEffect
{
private:
	unsigned int BlurryDivider1 = 5;
	unsigned int BlurryDivider2 = 10;

    Framebuffer *pBlurFramebufferH;
    Framebuffer *pBlurFramebufferV;
    Framebuffer *pBlurFramebufferV2;

	ShaderProgram *HblurShader;
	ShaderProgram *VblurShader;

public:
	GaussianBlur(Box *canvas, ivec2 resolution, int stage);
	~GaussianBlur();

	void render(Texture* RenderResult);
};

