#pragma once
#include "../PostProcessingEffect.h"

class ContrastChanger : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;
    float fContrast;

public:
	ContrastChanger(Canvas *canvas, const float& contrast);
	~ContrastChanger();

	Texture* render(Texture* texture);

    void setContrast(const float& contrast);
    float getContrast();
};

