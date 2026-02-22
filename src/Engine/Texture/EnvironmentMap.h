#pragma once
#include "../3D/Camera3D.h"

#include <Graphics/Framebuffer.h>
#include <Graphics/TextureCube.h>
#include <functional>

template<typename T>
class tEnvironmentMap : public tTextureCube<T>
{
private:
	Framebuffer *pFramebuffer;

	Camera3D* pCaptureCamera;
    std::vector<mat4> vCaptureViews;

public:
	tEnvironmentMap(const ivec2& resolution, std::string name = "EnvironmentMap");
	~tEnvironmentMap();

	void render(std::function<void()> renderfunc);

    ivec2 getSize() const;
    void setSize(const ivec2& size);
    Framebuffer* getFramebuffer();
};

template class tEnvironmentMap<unsigned char>;
template class tEnvironmentMap<float>;

typedef tEnvironmentMap<unsigned char> EnvironmentMap;
typedef tEnvironmentMap<float> EnvironmentMapf;