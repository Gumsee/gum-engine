#pragma once
#include "PostProcessingEffect.h"

class Brightfilter : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;

public:
	Brightfilter(Box* canvas, ivec2 resolution);
	~Brightfilter();

	void render(Texture* texture);
};