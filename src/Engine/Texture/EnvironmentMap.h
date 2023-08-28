#pragma once
#include "../3D/Camera3D.h"

#include <Graphics/Framebuffer.h>
#include <Graphics/TextureCube.h>
#include <functional>

class EnvironmentMap : public TextureCube
{
private:
	Framebuffer *pFramebuffer;

	Camera3D* pCaptureCamera;
    std::vector<mat4> vCaptureViews;

public:
	EnvironmentMap(const ivec2& resolution, std::string name = "EnvironmentMap", const unsigned short& datatype = Gum::Graphics::Datatypes::UNSIGNED_CHAR);
	~EnvironmentMap();

	void render(std::function<void()> renderfunc);

    ivec2 getSize() const;
    void setSize(const ivec2& size);
    Framebuffer* getFramebuffer();
};