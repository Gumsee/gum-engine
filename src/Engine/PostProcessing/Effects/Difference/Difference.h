#pragma once
#include "../PostProcessingEffect.h"

class Difference : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;
	
    Texture* pSecondTexture;

public:
	Difference(Box *canvas);
	~Difference();

	Texture* render(Texture* texture) override;

    void setSecondTexture(Texture* texture) { pSecondTexture = texture; }
};

