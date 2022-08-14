#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/TextureCube.h>

#include "../Managers/ObjectManager.h"

class EnvironmentMap
{
private:
	TextureCube *pResultTexture;
	Framebuffer *pFramebuffer;
    ObjectManager* pObjectManager;

	mat4 captureProjection;
    std::vector<mat4> captureViews;

public:
	EnvironmentMap(ObjectManager* objmanager);
	~EnvironmentMap();

	void render();

	Texture *getTexture();
};