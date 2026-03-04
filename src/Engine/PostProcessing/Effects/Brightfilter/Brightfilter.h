#pragma once
#include "../PostProcessingEffect.h"

class Brightfilter : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;

public:
	Brightfilter(Canvas* canvas);
	~Brightfilter();

	Texture* render(Texture* texture);
};