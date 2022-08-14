#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/TextureCube.h>
#include <Maths/mat.h>

class ReflectiveTextureCube
{
private:
    ivec2 v2Resolution;
	vec3 v3Position;

	//Temporary camera for rendering the Cubemap
	void rotateView(int Case);
	vec3 CurrentViewDirection;
	mat4 ViewMatrix;
	mat4 projectionMatrix;

	TextureCube *pResultTexture;
	Framebuffer *pFramebuffer;

public:
	ReflectiveTextureCube(ivec2 resolution = ivec2(1000, 1000));
	~ReflectiveTextureCube();

	void render();

	Texture *getTexture();
	void setPosition(vec3 pos);
	vec3 getPosition();
};

namespace Extra
{
	/*
	* Adds a ReflectiveTextureCube to the Texture Manager
	* This takes a vector3 pointer, so it can be applied to the camera's position
	* 
	* Note: The texture gets automatically updated by the Engine, no needs to update it Manually
	*/
	extern ReflectiveTextureCube *CubeReflectionTexture(vec3 position);
}