#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/Texture2D.h>

#include "../General/Renderer3D.h"

class ReflectiveTexture2D
{
private:
    ivec2 v2Resolution;
	Framebuffer* pFrameBuffer;
    Renderer3D* pRenderer;

	vec4 v4Plane;
	float fHeight;
    
public:
	ReflectiveTexture2D(Renderer3D* renderer, ivec2 resolution = ivec2(1280, 720));
	~ReflectiveTexture2D();

	void render(Camera* camera);
    
    //Getter
	Texture2D *getTexture();
	Texture2D *getDepthTexture();
	float getHeight();
	vec4 *getClippingPlane();

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
	extern ReflectiveTexture2D *planeReflectionTexture(float height);
}