#pragma once
#include "../Effects/PostProcessingEffect.h"

class HighDynamicRange : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;
	using PostProcessingEffect::render;

public:
	HighDynamicRange(Box *canvas, ivec2 resolution);
	~HighDynamicRange();

	void render(Texture* texture, float exposure);
};

