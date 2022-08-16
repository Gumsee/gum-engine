#pragma once
#include "PostProcessingEffect.h"
#include <type_traits>

class CombineTextures : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;
	using PostProcessingEffect::render;

public:
	CombineTextures(Box *canvas);
	~CombineTextures();
	void render(Texture* texture1, Texture* texture2, float exposure);
};