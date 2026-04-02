#include "EnvironmentMap.h"
#include <Graphics/WrapperFunctions.h>
#include "Graphics/Framebuffer.h"
#include "Graphics/TextureCube.h"

template<typename T>
tEnvironmentMap<T>::tEnvironmentMap(const ivec2& resolution, std::string name)
 : tTextureCube<T>(name)
{
    pFramebuffer = new Framebuffer(resolution);

    this->clampToEdge();
    this->setFiltering(Texture::FilteringType::LINEAR);
    this->setSize(resolution);

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

template<typename T>
tEnvironmentMap<T>::~tEnvironmentMap()
{
}

template<typename T>
void tEnvironmentMap<T>::render(std::function<void()> renderfunc)
{
	  Camera* oldcam = Camera::getActiveCamera();
    Framebuffer* oldFramebuffer = Framebuffer::CurrentlyBoundFramebuffer;
    pFramebuffer->bind();
    pCaptureCamera->makeActive();

    for (uint16_t i = 0; i < 6; ++i)
    {
		    pCaptureCamera->overrideViewMatrix(vCaptureViews[i]);
        pFramebuffer->attachTexture(0, this, Framebuffer::TextureTargets::CUBEMAP_POSITIVE_X + i, this->iCurrentMipmapLevel);
        pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH);

        renderfunc();
    }

    oldcam->makeActive();
    if(oldFramebuffer != nullptr)
      oldFramebuffer->bind();
}

//	GumEngine::Objects->render(GumEngine::Objects->WITHOUTREFLECTIVE);

template<typename T>
void tEnvironmentMap<T>::setSize(const ivec2& size)
{
    tTextureCube<T>::setSize(size);
    pFramebuffer->setSize(size);
}
template<typename T> ivec2 tEnvironmentMap<T>::getSize() const         { return pFramebuffer->getSize(); }

template<typename T> Framebuffer* tEnvironmentMap<T>::getFramebuffer() { return pFramebuffer; }

template class tEnvironmentMap<unsigned char>;
template class tEnvironmentMap<float>;