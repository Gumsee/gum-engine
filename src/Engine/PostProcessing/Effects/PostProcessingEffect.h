#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/ShaderProgram.h>
#include <GUI/Primitives/Box.h>

class PostProcessingEffect
{
protected:
	Box *pRenderCanvas;
	Framebuffer *pFramebuffer;
    void init(Box* canvas);

public:
    PostProcessingEffect();
    virtual ~PostProcessingEffect();

	virtual void render(Texture* texture);

	Texture* getResultTexture();

	virtual void setSize(ivec2 size);
};