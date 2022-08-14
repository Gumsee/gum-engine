#pragma once
#include "PostProcessingEffect.h"

class ContrastChanger : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;

public:
	ContrastChanger(Box *canvas, ivec2 resolution);
	~ContrastChanger();

	void render(Texture* texture, int contrast);
};

