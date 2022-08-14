#include "ObjectManager.h"
#include "ShaderManager.h"
#include "../General/Camera.h"
#include <string>


std::string ObjectManager::MODEL_ASSETS_PATH = "";
int ObjectManager::LAST_OBJECT_INSTANCE_ID = 0;

ObjectManager::ObjectManager(vec3 *sunDirection) 	
{
	Gum::Output::info("Initializing Object Manager...");

    pSkyBox = new SkyBox(Mesh::generateUVSphere(5, 15), sunDirection, "Skybox");
}
ObjectManager::~ObjectManager() {}


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

	for (auto &obj : Objects)
	{
        if(shader == nullptr)
		    obj.second->getShader()->use();
        if(noPrepare)
        {
            obj.second->renderMesh();
        }
        else
		    obj.second->render();
		/*if (obj.second->getPosition().y < Settings::getSetting(Settings::MAXIMUMFALLDISTANCE))
		{
			obj.second->clean();
		}*/
	}

	for (auto &obj : AnimObjs)
	{
        if(shader == nullptr)
		    obj.second->getShader()->use();
		obj.second->render();
	}
	glUseProgram(0);
}


void ObjectManager::renderToGBuffer(ShaderProgram* gbufferShader)
{
	for (auto &obj : Objects)
	{
        if(obj.second->getShader() == gbufferShader)
		    obj.second->render();
	}

	for (auto &obj : AnimObjs)
	{
        if(obj.second->getShader() == gbufferShader)
		    obj.second->render();
	}
}

void ObjectManager::renderExceptGBuffer(ShaderProgram* gbufferShader, Camera* camera)
{
	for (auto &obj : Objects)
	{
        if(obj.second->getShader() != gbufferShader)
        {
            obj.second->getShader()->use();
			obj.second->getShader()->LoadUniform("viewMatrix", camera->getViewMatrix());
			obj.second->getShader()->LoadUniform("projectionMatrix", camera->getProjectionMatrix());
		    obj.second->render();
        }
	}

	for (auto &obj : AnimObjs)
	{
        if(obj.second->getShader() != gbufferShader)
        {
            obj.second->getShader()->use();
			obj.second->getShader()->LoadUniform("viewMatrix", camera->getViewMatrix());
			obj.second->getShader()->LoadUniform("projectionMatrix", camera->getProjectionMatrix());
		    obj.second->render();
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
	
	Objects[Identifier] = obj;
	return obj;
}


void ObjectManager::clean()
{
	pSkyBox->clean();

	//TODO delete every single object


	for(auto &obj : Objects)
	{
		obj.second->clean();
		//obj.second.erase(obj.second.begin() + i);	
	}

	AnimObjs.clear();
	Objects.clear();
}

Object* ObjectManager::getObject(const std::string& name)
{
	if (Objects.find(name) != Objects.end())
	{
		if (Objects[name] != nullptr)
		{
			return Objects[name];
		}
		else
		{
			Gum::Output::error("ObjectManager: Texture Object: " + name + " doesn't exist!");
		}
	}
    Gum::Output::error("ObjectManager: Texture Object " + name + " doesn't exist!");
    return nullptr;
}

Object* ObjectManager::getObject(const unsigned int& vaoID)
{
	for (auto &obj : Objects)
	{
		if (obj.second->getVertexArrayObject()->getID() == vaoID)
		{
			return obj.second;
		}
	}

	return nullptr;
}

bool ObjectManager::hasObject(std::string name)
{
	return Objects.find(name) != Objects.end();
}

AnimatedModel* ObjectManager::getAnimObject(std::string name)
{
	if (AnimObjs.find(name) != AnimObjs.end())
	{
		if ((AnimObjs[name]) != nullptr)
		{
			return (AnimObjs[name]);
		}
		else
		{
			Gum::Output::error("ObjectManager: Animated Object: " + name + " doesn't exist!");
		}
	}
    Gum::Output::error("ObjectManager: Animated Object " + name + " doesn't exist!");
    return nullptr;
}

void ObjectManager::removeObject(Object *objToDelete)
{
	
}

Instance* ObjectManager::getInstanceByID(const unsigned int& id)
{
	for (auto &obj : Objects)
	{
		for(int i = 0; i < obj.second->numInstances(); i++)
		{
			if (obj.second->getInstance(i)->id == id)
			{
				return obj.second->getInstance(i);
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

	vec2 pos = Input::Mouse->getPosition() / renderer->getRenderCanvas()->getSize();
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
std::map<std::string, Object*> ObjectManager::getObjectsMap() { return this->Objects; }