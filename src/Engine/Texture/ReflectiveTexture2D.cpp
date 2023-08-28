#include "ReflectiveTexture2D.h"
#include "Graphics/Framebuffer.h"

ReflectiveTexture2D::ReflectiveTexture2D(Renderer3D* renderer, ivec2 resolution)
{
    this->pRenderer = renderer;
    v2Resolution = resolution;
    pFrameBuffer = new Framebuffer(resolution);
    pFrameBuffer->addTextureAttachment(0, "ReflectionTexture2D");
    pFrameBuffer->addDepthTextureAttachment("ReflectionDepthTexture2D");
    pFrameBuffer->addDepthAttachment();
}

ReflectiveTexture2D::~ReflectiveTexture2D()
{
}

void ReflectiveTexture2D::render(Camera* camera)
{
	//glEnable(GL_CLIP_DISTANCE0);
	pFrameBuffer->bind();
    pFrameBuffer->clear(Framebuffer::ClearFlags::COLOR | Framebuffer::ClearFlags::DEPTH);
	float distance = 2 * (camera->getPosition().y - this->fHeight);
	//vec3 oldpos = camera->getPosition();
	//mat4 oldmat = camera->getViewMatrix();

	camera->setPosition(camera->getPosition() + vec3(0, -distance, 0));
	camera->invertPitch();
	v4Plane = vec4(0, 1, 0, -this->fHeight);
	//GumEngine::Shaders->setClipPlane(v4Plane);
	//GumEngine::Shaders->update("viewMatrix");
	//GumEngine::Shaders->update("ClipPlane");
	//GumEngine::Shaders->update();

	//pRenderer->render(ObjectManager::WITHOUTREFLECTIVE);

	//GumEngine::ActiveCamera->viewDirection.y *= -1;
	//GumEngine::ActiveCamera->viewMat = oldmat;
	v4Plane = vec4(0);
	//GumEngine::Shaders->setClipPlane(v4Plane);
	camera->setPosition(camera->getPosition() + vec3(0, distance, 0));
	//GumEngine::Shaders->update("viewMatrix");
	//GumEngine::Shaders->update("ClipPlane");
	//GumEngine::Shaders->update();
	pFrameBuffer->unbind();
	//glDisable(GL_CLIP_DISTANCE0);
}

Texture2D* ReflectiveTexture2D::getTexture()      { return (Texture2D*)pFrameBuffer->getTextureAttachment(); }
TextureDepth* ReflectiveTexture2D::getDepthTexture() { return pFrameBuffer->getDepthTextureAttachment(); }
float ReflectiveTexture2D::getHeight()            { return fHeight; }
vec4* ReflectiveTexture2D::getClippingPlane()     { return &v4Plane; }


void ReflectiveTexture2D::setHeight(float height) { this->fHeight = height; }

namespace Extra
{
	ReflectiveTexture2D *planeReflectionTexture(float height)
	{
		ReflectiveTexture2D *tex;
		//tex = new ReflectiveTexture2D(GumEngine::DefaultRenderer);
		tex->setHeight(height);

		//GumEngine::Textures->addRenderable(tex);

		return tex;
	}
}