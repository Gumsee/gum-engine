#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/ShaderProgram.h>
#include <GUI/Primitives/Box.h>
#include <Maths/mat.h>
#include <Maths/vec.h>

#include "../../Lightning/Light.h"
#include "../../Lightning/G_Buffer.h"

class Renderer3D;
class ShadowMapping;
class World3D;

class Lightning
{
private:
    Box* pRenderCanvas;
	ShaderProgram *pShader;
    void initShader();
	Renderer3D *pRenderer;

	vec2 pixelSize;
	long long microseconds;

public:
	Lightning(Box* canvas, Renderer3D *renderer);
	~Lightning();

	void render(ShadowMapping *shadowmap, World3D* world);


	long long getExecutionTime();
	
	void loadLight(Light* light, int index);
};