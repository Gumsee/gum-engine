#pragma once
#include "PostProcessingEffect.h"

class CombineTextures : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;

public:
	CombineTextures(Box *canvas, ivec2 resolution);
	~CombineTextures();
	void render(Texture* texture1, Texture* texture2, float exposure);
};

