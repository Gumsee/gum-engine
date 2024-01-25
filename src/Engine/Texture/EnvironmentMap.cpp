#include "EnvironmentMap.h"
#include <Graphics/WrapperFunctions.h>
#include "../Shaders/ShaderManager.h"
#include "../3D/Renderer3D.h"
#include "../3D/World3D.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/TextureCube.h"
#include "Graphics/Variables.h"

EnvironmentMap::EnvironmentMap(const ivec2& resolution, std::string name, const unsigned short& datatype)
 : TextureCube(name, datatype)
{
    pFramebuffer = new Framebuffer(resolution);

    clampToEdge();
    setFiltering(Texture::FilteringTypes::LINEAR);
    setSize(resolution);

    pFramebuffer->addDepthAttachment();
    pFramebuffer->addCubeTextureAttachment(0, this);

    pCaptureCamera = new Camera3D(pFramebuffer->getSize(), nullptr);
    pCaptureCamera->setFOV(90.0f);

	vCaptureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)));
	vCaptureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)));
	vCaptureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)));
	vCaptureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)));
	vCaptureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f)));
	vCaptureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)));
}

EnvironmentMap::~EnvironmentMap()
{
}

void EnvironmentMap::render(std::function<void()> renderfunc)
{
	Camera* oldcam = Camera::getActiveCamera();
    Framebuffer* oldFramebuffer = Framebuffer::CurrentlyBoundFramebuffer;
    pFramebuffer->bind();
    pCaptureCamera->makeActive();

    for (unsigned int i = 0; i < vCaptureViews.size(); ++i)
    {
		pCaptureCamera->overrideViewMatrix(vCaptureViews[i]);
        pFramebuffer->attachTexture(0, this, Framebuffer::TextureTargets::CUBEMAP_POSITIVE_X + i, iCurrentMipmapLevel);
        pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH);

        renderfunc();
    }

    oldcam->makeActive();
    oldFramebuffer->bind();
}

//	GumEngine::Objects->render(GumEngine::Objects->WITHOUTREFLECTIVE);

void EnvironmentMap::setSize(const ivec2& size)
{
    TextureCube::setSize(size);
    pFramebuffer->setSize(size);
}
ivec2 EnvironmentMap::getSize() const         { return pFramebuffer->getSize(); }

Framebuffer* EnvironmentMap::getFramebuffer() { return pFramebuffer; }