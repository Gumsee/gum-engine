#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/ShaderProgram.h>
#include <GUI/Primitives/Box.h>

class PostProcessingEffect
{
protected:
	Box *pRenderCanvas;
	Framebuffer *pFramebuffer;
    void init(Box* canvas, ivec2 resolution);

public:
    PostProcessingEffect();
    ~PostProcessingEffect();

	virtual void render(Texture* texture);

	Texture* getResultTexture();
};