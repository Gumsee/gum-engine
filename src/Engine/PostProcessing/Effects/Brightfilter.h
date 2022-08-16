#pragma once
#include "PostProcessingEffect.h"

class Brightfilter : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;

public:
	Brightfilter(Box* canvas);
	~Brightfilter();

	void render(Texture* texture);
};