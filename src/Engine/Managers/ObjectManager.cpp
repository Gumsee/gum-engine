#include "ObjectManager.h"
#include "ShaderManager.h"
#include "../General/Camera.h"
#include "../General/Renderer3D.h"
#include <System/MemoryManagement.h>
#include <string>


std::string ObjectManager::MODEL_ASSETS_PATH = "";
int ObjectManager::LAST_OBJECT_INSTANCE_ID = 0;

ObjectManager::ObjectManager(vec3 *sunDirection) 	
{
	Gum::Output::info("Initializing Object Manager...");

    pSkyBox = new SkyBox(Mesh::generateUVSphere(5, 15), sunDirection, "Skybox");
}
ObjectManager::~ObjectManager() 
{
	Gum::_delete(pSkyBox);

	for(Object* obj : vObjects)
		Gum::_delete(obj);
		
	for(Object* obj : vAnimObjs)
		Gum::_delete(obj);

	vAnimObjs.clear();
	vObjects.clear();
}


void ObjectManager::render(int exception, ShaderProgram *shader, bool noPrepare)
{
    if(shader != nullptr)
        shader->use();
    if(pSkyBox != nullptr && !(exception & ExceptionTypes::WITHOUTSKYBOX))
    {
        if(shader == nullptr)
            pSkyBox->getShader()->use();
        if(noPrepare)
        {
            pSkyBox->renderMesh();
        }
        else
            pSkyBox->render();
    }

	for (Object* obj : vObjects)
	{
        if(shader == nullptr)
		    obj->getShader()->use();
        if(noPrepare)
        {
            obj->renderMesh();
        }
        else
		    obj->render();
		/*if (obj.second->getPosition().y < Settings::getSetting(Settings::MAXIMUMFALLDISTANCE))
		{
			obj.second->clean();
		}*/
	}

	for (AnimatedModel* obj : vAnimObjs)
	{
        if(shader == nullptr)
		    obj->getShader()->use();
		obj->render();
	}
	glUseProgram(0);
}


void ObjectManager::renderToGBuffer(ShaderProgram* gbufferShader)
{
	for (Object* obj : vObjects)
	{
        if(obj->getShader() == gbufferShader)
		    obj->render();
	}

	for (AnimatedModel* obj : vAnimObjs)
	{
        if(obj->getShader() == gbufferShader)
		    obj->render();
	}
}

void ObjectManager::renderExceptGBuffer(ShaderProgram* gbufferShader, Camera* camera)
{
	for (Object* obj : vObjects)
	{
        if(obj->getShader() != gbufferShader)
        {
            obj->getShader()->use();
			obj->getShader()->loadUniform("viewMatrix", camera->getViewMatrix());
			obj->getShader()->loadUniform("projectionMatrix", camera->getProjectionMatrix());
		    obj->render();
        }
	}

	for (AnimatedModel* obj : vAnimObjs)
	{
        if(obj->getShader() != gbufferShader)
        {
            obj->getShader()->use();
			obj->getShader()->loadUniform("viewMatrix", camera->getViewMatrix());
			obj->getShader()->loadUniform("projectionMatrix", camera->getProjectionMatrix());
		    obj->render();
        }
	}
	glUseProgram(0);
}


Object* ObjectManager::addObject(Object *obj, std::string Identifier)
{
    if(Identifier == "")
        Identifier = obj->getName();
		
    if(obj->getShader() == nullptr)
    	 obj->setShader(Gum::ShaderManager::getShaderProgram("GBufferShader"));
	
	vObjects.push_back(obj);
	return obj;
}


Object* ObjectManager::getObject(const std::string& name)
{
	for(Object* obj : vObjects)
	{
		if (obj->getName() == name)
			return obj;
	}
    Gum::Output::error("ObjectManager: Texture Object " + name + " doesn't exist!");
    return nullptr;
}

Object* ObjectManager::getObject(const unsigned int& index)
{
	return vObjects[index];
}

bool ObjectManager::hasObject(std::string name)
{
	return getObject(name) != nullptr;
}

AnimatedModel* ObjectManager::getAnimObject(std::string name)
{
	for(AnimatedModel* obj : vAnimObjs)
	{
		if (obj->getName() == name)
			return obj;
	}
    Gum::Output::error("ObjectManager: Animated Object " + name + " doesn't exist!");
    return nullptr;
}

void ObjectManager::removeObject(Object *objToDelete)
{
	
}

Instance* ObjectManager::getInstanceByID(const unsigned int& id)
{
	for(Object* obj : vObjects)
	{
		for(int i = 0; i < obj->numInstances(); i++)
		{
			if (obj->getInstance(i)->id == id)
			{
				return obj->getInstance(i);
			}
		}
	}
    Gum::Output::warn("ObjectManager: Object with ID " + std::to_string(id) + " doesn't exist!");
    return nullptr;
}

//
// Super slow:
//
unsigned int ObjectManager::getObjectUnderMouse(Renderer3D* renderer) const
{
	renderer->getGBuffer()->getFramebuffer()->bind();
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	//unsigned int check = glGetError();
	glReadBuffer(GL_COLOR_ATTACHMENT1);

	vec2 pos = renderer->getContextWindow()->getMouse()->getPosition() / renderer->getRenderCanvas()->getSize();
	pos.y = 1 - pos.y;
	pos *= renderer->getRenderCanvas()->getSize();
	unsigned char data[4];
	glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); //slow
	renderer->getGBuffer()->getFramebuffer()->unbind();
	int pickedID = data[0] + data[1] * 256 + data[2] * 256*256;

	return pickedID;
}


//
// Setter
//
void ObjectManager::setSkybox(SkyBox *skybox)                 { this->pSkyBox = skybox; }


//
//Getter
//
SkyBox* ObjectManager::getSkybox() 			                  { return this->pSkyBox; }
unsigned int ObjectManager::numObjects() 					  { return this->vObjects.size(); }