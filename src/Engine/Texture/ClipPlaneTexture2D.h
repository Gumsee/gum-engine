#pragma once
#include <iostream>
#include <GL/glew.h>
#include <Graphics/Framebuffer.h>

#include "../3D/Renderer3D.h"

class ClipPlaneTexture2D
{
private:
	vec4 v4Pplane;
	float fHeight;
	Framebuffer* pFramebuffer;
    ivec2 v2Resolution;
    Renderer3D* pRenderer;

public:
	ClipPlaneTexture2D(Renderer3D* renderer, ivec2 resolution = ivec2(1280, 720));
	~ClipPlaneTexture2D();

	void render();

    //Getter
    Texture* getTexture();
    Texture* getDepthTexture();
    float getHeight();
    vec4* getClippingPlane();

    //Setter
    void setHeight(float height);
};

namespace Extra
{
	/*
	* Adds a ReflectiveTexture2D to the Texture Manager
	* This only works on the Z axis
	* 
	* Note: The texture gets automatically updated by the Engine, no needs to update it Manually
	*/
	extern ClipPlaneTexture2D *ClipPlaneTexture(float height);
}