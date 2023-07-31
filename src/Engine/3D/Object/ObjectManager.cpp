#include "ObjectManager.h"
#include "Desktop/Window.h"
#include "../../Shaders/ShaderManager.h"
#include "../Camera3D.h"
#include "../Renderer3D.h"
#include "System/Output.h"
#include <System/MemoryManagement.h>
#include <string>


std::string ObjectManager::MODEL_ASSETS_PATH = "";

ObjectManager::ObjectManager(vec3 *sunDirection) 	
{
	Gum::Output::info("Initializing Object3D Manager...");

    pSkyBox = new SkyBox(Mesh::generateUVSphere(5, 15), sunDirection, "Skybox");
}
ObjectManager::~ObjectManager() 
{
	Gum::_delete(pSkyBox);

	for(Object3D* obj : vObjects)
		Gum::_delete(obj);

	vObjects.clear();
}

void ObjectManager::render(int exception, ShaderProgram *shader, bool noPrepare)
{
    if(shader != nullptr)
        shader->use();
    if(pSkyBox != nullptr && !(exception & ExceptionTypes::WITHOUTSKYBOX))
    {
        if(shader == nullptr)
            pSkyBox->getShaderProgram()->use();
        if(noPrepare)
        {
            pSkyBox->renderMesh();
        }
        else
            pSkyBox->render();
    }

	for (Object3D* obj : vObjects)
	{
        if(shader == nullptr)
		    obj->getShaderProgram()->use();
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
	glUseProgram(0);
}


void ObjectManager::renderToGBuffer(ShaderProgram* gbufferShader)
{
	for (Object3D* obj : vObjects)
	{
        if(obj->getShaderProgram() == gbufferShader)
		    obj->render();
	}
}

void ObjectManager::renderExceptGBuffer(ShaderProgram* gbufferShader, Camera* camera)
{
	for (Object3D* obj : vObjects)
	{
        if(obj->getShaderProgram() != gbufferShader)
        {
            obj->getShaderProgram()->use();
			obj->getShaderProgram()->loadUniform("viewMatrix", camera->getViewMatrix());
			obj->getShaderProgram()->loadUniform("projectionMatrix", camera->getProjectionMatrix());
		    obj->render();
        }
	}
	glUseProgram(0);
}


Object3D* ObjectManager::addObject(Object3D *obj, std::string Identifier)
{
    if(Identifier == "")
        Identifier = obj->getName();
		
    if(obj->getShaderProgram() == nullptr)
         obj->setShaderProgram(Gum::ShaderManager::getShaderProgram("GBufferShader"));
	
	vObjects.push_back(obj);
    if(pAddObjectCallback != nullptr)
        pAddObjectCallback(obj);

    return obj;
}


Object3D* ObjectManager::getObject(const std::string& name)
{
	for(Object3D* obj : vObjects)
	{
		if (obj->getName() == name)
			return obj;
	}
    Gum::Output::error("ObjectManager: Texture Object3D " + name + " doesn't exist!");
    return nullptr;
}

Object3D* ObjectManager::getObject(const unsigned int& index)
{
	return vObjects[index];
}

bool ObjectManager::hasObject(std::string name)
{
	return getObject(name) != nullptr;
}

void ObjectManager::removeObject(Object3D *objToDelete)
{
	
}

Object3DInstance* ObjectManager::getInstanceByID(const unsigned int& id)
{
	for(Object3D* obj : vObjects)
	{
		for(unsigned int i = 0; i < obj->numInstances(); i++)
		{
			if (obj->getInstance(i)->getID() == id)
			{
				return obj->getInstance(i);
			}
		}
	}
    Gum::Output::warn("ObjectManager: Object3D with ID " + std::to_string(id) + " doesn't exist!");
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

	vec2 pos = Gum::Window::CurrentlyBoundWindow->getMouse()->getPosition() / renderer->getRenderCanvas()->getSize();
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
void ObjectManager::onAddObject(AddObjectCallback callback)   { this->pAddObjectCallback = callback; }


//
//Getter
//
SkyBox* ObjectManager::getSkybox() 			                  { return this->pSkyBox; }
unsigned int ObjectManager::numObjects() 					  { return this->vObjects.size(); }