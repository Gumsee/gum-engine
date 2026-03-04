#pragma once
#include "../PostProcessingEffect.h"

class CombineTextures : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;

    Texture* pSecondTexture;
    float fExposure;

public:
	CombineTextures(Canvas *canvas, const float& exposure = 1.0f);
	~CombineTextures();


	Texture* render(Texture* texture);

    void setSecondTexture(Texture* texture) { pSecondTexture = texture; }
    void setExposure(const float& exposure) { fExposure = exposure; }
};