#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/ShaderProgram.h>
#include <Maths/mat.h>
#include <Maths/vec.h>

#include "../../Lightning/Light.h"
#include "../../Lightning/G_Buffer.h"
#include "../../Rendering/Canvas.h"

class ShadowMapping;
class World3D;

class Lightning
{
private:
    Canvas* pRenderCanvas;
	static inline ShaderProgram *pShader = nullptr;

	vec2 pixelSize;
	long long microseconds;

public:
	Lightning(Canvas* canvas);
	~Lightning();

	void updateShader(ShadowMapping *shadowmap, World3D* world);


	long long getExecutionTime();
	
	void loadLight(Light* light, int index);
    static void initShader();
    static ShaderProgram* getDefaultShaderProgram();
};