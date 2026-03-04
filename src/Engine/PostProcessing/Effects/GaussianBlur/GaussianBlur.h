#pragma once
#include "../PostProcessingEffect.h"

class GaussianBlur : public PostProcessingEffect
{
private:
	unsigned int BlurryDivider1 = 5;
	unsigned int BlurryDivider2 = 10;

    Framebuffer *pBlurFramebufferH;
    Framebuffer *pBlurFramebufferV;
    Framebuffer *pBlurFramebufferV2;

	static inline ShaderProgram *HblurShader = nullptr;
	static inline ShaderProgram *VblurShader = nullptr;

public:
	GaussianBlur(Canvas *canvas, int stage);
	~GaussianBlur();

	Texture* render(Texture* RenderResult);
};

