#pragma once
#include "PostProcessingEffect.h"

class Difference : public PostProcessingEffect
{
private:
	ShaderProgram *pShader;
	
public:
	Difference(Box *canvas);
	~Difference();

	void render(Texture* texture1, Texture* texture2);
};

