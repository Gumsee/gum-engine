#pragma once
#include "../PostProcessingEffect.h"

class Brightfilter : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;

public:
	Brightfilter(Box* canvas);
	~Brightfilter();

	Texture* render(Texture* texture);
};