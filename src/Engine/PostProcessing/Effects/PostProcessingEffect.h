#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/ShaderProgram.h>
#include "../../Rendering/Canvas.h"

class PostProcessingEffect
{
protected:
	Canvas *pRenderCanvas;
	Framebuffer *pFramebuffer;
    void init(Canvas* canvas);

public:
    PostProcessingEffect();
    virtual ~PostProcessingEffect();

	virtual Texture* render(Texture* texture);

	virtual void setSize(ivec2 size);

  Framebuffer* getFramebuffer();
};