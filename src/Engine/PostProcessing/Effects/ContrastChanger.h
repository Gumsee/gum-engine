#pragma once
#include "PostProcessingEffect.h"

class ContrastChanger : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;
	using PostProcessingEffect::render;

public:
	ContrastChanger(Box *canvas);
	~ContrastChanger();

	void render(Texture* texture, int contrast);
};

