#pragma once
#include "Engine/Managers/ObjectManager.h"
#include <OpenGL/Framebuffer.h>
#include <OpenGL/TextureCube.h>

class Renderer3D;

class EnvironmentMap
{
private:
	TextureCube *pResultTexture;
	Framebuffer *pFramebuffer;
    Renderer3D* pRenderer;

	mat4 captureProjection;
    std::vector<mat4> captureViews;

public:
	EnvironmentMap(Renderer3D* renderer);
	~EnvironmentMap();

	void render();

	Texture *getTexture();
};