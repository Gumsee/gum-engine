#pragma once
#include "../PostProcessingEffect.h"

class MotionBlur : public PostProcessingEffect
{
private:
	static inline ShaderProgram *pShader = nullptr;

  vec3 vOldCameraDirection;

public:
	MotionBlur(Canvas* canvas);
	~MotionBlur();

	Texture* render(Texture* texture);
};