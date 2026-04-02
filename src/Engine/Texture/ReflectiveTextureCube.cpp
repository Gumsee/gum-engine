#include "ReflectiveTextureCube.h"
#include <Graphics/ShaderProgram.h>
#include <gum-maths.h>
#include <System/Output.h>

ReflectiveTextureCube::ReflectiveTextureCube(ivec2 resolution)
{
    pFramebuffer = new Framebuffer(resolution);
    pFramebuffer->addDepthAttachment();
    pFramebuffer->bind();
    pFramebuffer->unbind();

	pResultTexture = new TextureCube("ReflectionTextureCube");
	projectionMatrix = Gum::Maths::perspective(90.0f, 1.0f, 0.1f, 1000.0f);
}

ReflectiveTextureCube::~ReflectiveTextureCube()
{
}

void ReflectiveTextureCube::render()
{
	/// IN WORK 
	// Render in all 6 directions

	//ShaderProgram *shader = ShaderProgram::getShaderProgramByName("lowquality");
	#ifdef DEBUG
		Gum::Output::debug("Rendering ReflectiveTexture Cube!");
	#endif
	pFramebuffer->bind();
	//glClearDepth(1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pResultTexture->bind();
	for(int i = 0; i < 6; i++)
	{
		//shader->loadUniform("viewMatrix", ViewMatrix);
		//shader->loadUniform("projectionMatrix", projectionMatrix);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pResultTexture->getID() , 0);

		rotateView(i);
        //#error "eeh"
		//GumEngine::DefaultRenderer->getObjectManager()->render(0, shader);
	}
	pResultTexture->unbind();

	pFramebuffer->unbind();
}

void ReflectiveTextureCube::rotateView(int Case)
{
	switch(Case)
	{
		case 0:
			CurrentViewDirection.x = 1;
			CurrentViewDirection.y = 0;
			CurrentViewDirection.z = 0;
			break;

		case 1:
			CurrentViewDirection.x = -1;
			CurrentViewDirection.y = 0;
			CurrentViewDirection.z = 0;
			break;

		case 2:
			CurrentViewDirection.x = 0;
			CurrentViewDirection.y = 1;
			CurrentViewDirection.z = 0;
			break;

		case 3:
			CurrentViewDirection.x = 0;
			CurrentViewDirection.y = -1;
			CurrentViewDirection.z = 0;
			break;

		case 4:
			CurrentViewDirection.x = 0;
			CurrentViewDirection.y = 0;
			CurrentViewDirection.z = 1;
			break;

		case 5:
			CurrentViewDirection.x = 0;
			CurrentViewDirection.y = 0;
			CurrentViewDirection.z = -1;
			break;
	}

	ViewMatrix = Gum::Maths::view(v3Position, v3Position + CurrentViewDirection, vec3(0, 1, 0));
}


Texture* ReflectiveTextureCube::getTexture()       { return pFramebuffer->getTextureAttachment(0); }
vec3 ReflectiveTextureCube::getPosition()          { return v3Position; }

void ReflectiveTextureCube::setPosition(vec3 pos) { this->v3Position = pos; }

namespace Extra
{
	ReflectiveTextureCube *CubeReflectionTexture(vec3 position)
	{
		ReflectiveTextureCube *tex;
		tex = new ReflectiveTextureCube();
		tex->setPosition(position);

		//GumEngine::Textures->addRenderable(tex);

		return tex;
	}
}