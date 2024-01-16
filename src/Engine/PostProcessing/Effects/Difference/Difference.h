#pragma once
#include "../PostProcessingEffect.h"

class Difference : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;
	
public:
	Difference(Box *canvas);
	~Difference();

	void render(Texture* texture1, Texture* texture2);
};

