#include "ClipPlaneTexture2D.h"

ClipPlaneTexture2D::ClipPlaneTexture2D(Renderer3D* renderer, ivec2 resolution)
{
    this->pRenderer = renderer;
    v2Resolution = resolution;
    pFramebuffer = new Framebuffer(resolution);
    pFramebuffer->addTextureAttachment<unsigned char>(0, "ReflectionTexture2D");
    pFramebuffer->addDepthTextureAttachment("ReflectionDepthTexture2D");
    pFramebuffer->addDepthAttachment();
}

ClipPlaneTexture2D::~ClipPlaneTexture2D()
{
}

void ClipPlaneTexture2D::render()
{
	//glEnable(GL_CLIP_DISTANCE0);
	pFramebuffer->bind();
    pFramebuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH);

	//GumEngine::ActiveCamera->invertPitch();
	v4Pplane = vec4(0, -1, 0, this->fHeight);
	//GumEngine::Shaders->setClipPlane(v4Pplane);
	//GumEngine::ActiveCamera->update();
	//GumEngine::Shaders->update("ClipPlane");
	//GumEngine::Shaders->update();

	//pRenderer->getObjectManager()->render(ObjectManager::WITHOUTREFLECTIVE);

	v4Pplane = vec4(0);
	//GumEngine::Shaders->setClipPlane(v4Pplane);

	//GumEngine::ActiveCamera->invertPitch();
	pFramebuffer->unbind();
	//glDisable(GL_CLIP_DISTANCE0);
}

//Getter
Texture* ClipPlaneTexture2D::getTexture()        { return pFramebuffer->getTextureAttachment(0); }
Texture* ClipPlaneTexture2D::getDepthTexture()   { return pFramebuffer->getDepthTextureAttachment(); }
float ClipPlaneTexture2D::getHeight()            { return fHeight; }
vec4* ClipPlaneTexture2D::getClippingPlane()     { return &v4Pplane; }

//Setter
void ClipPlaneTexture2D::setHeight(float height) { this->fHeight = height; }