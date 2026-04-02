#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/Texture2D.h>

#include "../3D/Renderer3D.h"
#include "../Rendering/Camera.h"

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
	Texture *getDepthTexture();
	float getHeight();
	vec4 *getClippingPlane();

  //Setter
	void setHeight(float height);
};