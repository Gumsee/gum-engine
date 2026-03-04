#pragma once
#include "../Effects/PostProcessingEffect.h"

class HighDynamicRange : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;
	using PostProcessingEffect::render;

public:
	HighDynamicRange(Canvas* canvas);
	~HighDynamicRange();

	Texture* render(Texture* texture, float exposure);
};

