#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/TextureCube.h>

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