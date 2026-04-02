#pragma once
#include <iostream>
#include <Graphics/Framebuffer.h>

#include "../3D/Renderer3D.h"

class ClipPlaneTexture2D
{
private:
	vec4 v4Pplane;
	float fHeight;
	Framebuffer* pFramebuffer;
  Renderer3D* pRenderer;
  ivec2 v2Resolution;

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