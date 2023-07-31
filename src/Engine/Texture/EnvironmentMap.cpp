#include "EnvironmentMap.h"
#include <OpenGL/WrapperFunctions.h>
#include "../Shaders/ShaderManager.h"
#include "../3D/Renderer3D.h"
#include "../3D/World3D.h"

EnvironmentMap::EnvironmentMap(Renderer3D* renderer)
{
    this->pRenderer = renderer;
	pResultTexture = new TextureCube("EnvironmentMap");
    pResultTexture->bind(0);
    for (unsigned int i = 0; i < 6; ++i)
    {
        gumTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, ivec2(Settings::getSetting(Settings::Names::REFLECTION_SIZE)), 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    pResultTexture->unbind(0);


    pFramebuffer = new Framebuffer(ivec2(Settings::getSetting(Settings::Names::REFLECTION_SIZE), Settings::getSetting(Settings::Names::REFLECTION_SIZE)));
    pFramebuffer->addDepthAttachment();
    pFramebuffer->bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
    pFramebuffer->unbind();
	

	captureProjection = Gum::Maths::perspective(90.0f, 1.0f, 0.1f, 1000.0f);
	captureViews.reserve(6);
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, 1.0f,  0.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f,  0.0f,  0.0f), vec3(0.0f, 1.0f,  0.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, 1.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  -1.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,  0.0f,  1.0f), vec3(0.0f, 1.0f,  0.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,  0.0f, -1.0f), vec3(0.0f, 1.0f,  0.0f)));
}

EnvironmentMap::~EnvironmentMap()
{
}

void EnvironmentMap::render()
{
	glViewport(0, 0, Settings::getSetting(Settings::Names::REFLECTION_SIZE), Settings::getSetting(Settings::Names::REFLECTION_SIZE)); // don't forget to configure the viewport to the capture dimensions.
    pFramebuffer->bind();

	//mat4 oldview = GumEngine::ActiveCamera->getViewMatrix();
	//mat4 oldproj = GumEngine::ActiveCamera->getProjectionMatrix();

    for (unsigned int i = 0; i < captureViews.size(); ++i)
    {
		/*GumEngine::ActiveCamera->viewMat = captureViews[i];
		GumEngine::ActiveCamera->projectionMatrix = captureProjection;
		GumEngine::Shaders->update("viewMatrix");
		GumEngine::Shaders->update("projectionMatrix");*/
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pResultTexture->getID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		pRenderer->getWorld()->getObjectManager()->getSkybox()->getShaderProgram()->use();
		pRenderer->getWorld()->getObjectManager()->getSkybox()->render();
		pRenderer->getWorld()->getObjectManager()->getSkybox()->getShaderProgram()->unuse();

        //GumEngine::Objects->render(GumEngine::Objects->WITHOUTREFLECTIVE);
		pRenderer->getWorld()->getObjectManager()->render(ObjectManager::ExceptionTypes::WITHOUTSKYBOX, Gum::ShaderManager::getShaderProgram("ReflectionlessBasicShader"));
    }

	/*GumEngine::ActiveCamera->viewMat = oldview;
	GumEngine::ActiveCamera->projectionMatrix = oldproj;
	GumEngine::Shaders->update("viewMatrix");
	GumEngine::Shaders->update("projectionMatrix");*/

    pFramebuffer->unbind(pRenderer->getRenderCanvas()->getSize());
}

//	GumEngine::Objects->render(GumEngine::Objects->WITHOUTREFLECTIVE);

Texture* EnvironmentMap::getTexture() { return this->pResultTexture; }