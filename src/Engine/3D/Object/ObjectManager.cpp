#include "ObjectManager.h"
#include "Desktop/Window.h"
#include "../Lightning/ShadowMapping/ShadowMapping.h"
#include "../Camera3D.h"
#include "../Renderer3D.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/ShaderProgram.h"
#include "System/File.h"
#include "System/Output.h"
#include <System/MemoryManagement.h>
#include <string>


Gum::File ObjectManager::MODEL_ASSETS_PATH = Gum::File("", Gum::Filesystem::DIRECTORY);

ObjectManager::ObjectManager(vec3 *sunDirection) 	
{
	Gum::Output::info("Initializing Object3D Manager...");

    pSkyBox = new SkyBox(Mesh::generateUVSphere(5, 15), sunDirection, "Skybox");
}
ObjectManager::~ObjectManager() 
{
	Gum::_delete(pSkyBox);

	for(auto objs : mObjectsDefered)
        for(Object3D* obj : objs.second)
		    Gum::_delete(obj);

	for(auto objs : mObjectsForward)
        for(Object3D* obj : objs.second)
		    Gum::_delete(obj);

	mObjectsForward.clear();
	mObjectsDefered.clear();
}


void ObjectManager::renderSky()
{
    pSkyBox->getShaderProgram()->use();
    /*if(noPrepare) { pSkyBox->renderMesh(); }
    else*/ { pSkyBox->render(); }
    pSkyBox->getShaderProgram()->unuse();
}

void ObjectManager::renderDefered(G_Buffer* gbuffer, Box* rendercanvas)
{
    Framebuffer* currentFramebuffer = Framebuffer::CurrentlyBoundFramebuffer;

	for(auto objs : mObjectsDefered)
    {
        //Render Objects to GBuffer
        gbuffer->bind();
        gbuffer->getShader()->use();
        for(Object3D* obj : objs.second)
            obj->render();
        

        //Render given shader using gbuffer
        currentFramebuffer->bind();
        ShaderProgram* shader = objs.first;

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gbuffer->getPositionMap()->bind(0);
        gbuffer->getNormalMap()->bind(1);
        gbuffer->getDiffuseMap()->bind(2);
        //pRenderer->getSSAO()->getResultTexture()->bind(3);
        gbuffer->getObjectDataMap()->bind(4);
        //pRenderer->getEnvironmentMap()->getTexture()->bind(5);
        pSkyBox->getTexture()->bind(5);
        pSkyBox->getIrradianceMap()->bind(6);
        pSkyBox->getPreFilterMap()->bind(7);
        pSkyBox->getBRDFConvMap()->bind(8);
        ((Renderer3D*)Renderer3D::getActiveRenderer())->getShadowMapping()->getResultTexture()->bind(9);

        shader->use();
        rendercanvas->renderCustom();

        gbuffer->getFramebuffer()->blitDepthToOtherFramebuffer(currentFramebuffer);
    }

    ShaderProgram::unuse();
}


void ObjectManager::renderForward()
{
	for (auto objs : mObjectsForward)
	{
        objs.first->use();
        for(Object3D* obj : objs.second)
            obj->render();
	}
    ShaderProgram::unuse();
}


void ObjectManager::renderEverything()
{
	for(auto objs : mObjectsDefered)
        for(Object3D* obj : objs.second)
            obj->render();
        
	for(auto objs : mObjectsForward)
        for(Object3D* obj : objs.second)
            obj->render();
}


void ObjectManager::renderIDs()
{
	for(auto objs : mObjectsDefered)
        for(Object3D* obj : objs.second)
            obj->renderID();
        
	for(auto objs : mObjectsForward)
        for(Object3D* obj : objs.second)
            obj->renderID();
}


void ObjectManager::updateShaderPrograms(Camera* camera)
{
    pSkyBox->getShaderProgram()->use();
    pSkyBox->getShaderProgram()->loadUniform("projectionMatrix", camera->getProjectionMatrix());
    pSkyBox->getShaderProgram()->unuse();
	for(auto objs : mObjectsForward)
    {
        ShaderProgram* shader = objs.first;
        shader->use();
        shader->loadUniform("projectionMatrix", camera->getProjectionMatrix());
        shader->unuse();
    }

	for(auto objs : mObjectsDefered)
    {
        ShaderProgram* shader = objs.first;
        shader->use();
        shader->loadUniform("projectionMatrix", camera->getProjectionMatrix());
        shader->unuse();
    }
}


Object3D* ObjectManager::addObject(Object3D* obj, ShaderProgram* shader)
{		
    if(obj->getShaderProgram() == nullptr)
        obj->setShaderProgram(shader);
	
    mObjectsDefered[shader].push_back(obj);

    if(pAddObjectCallback != nullptr)
        pAddObjectCallback(obj);

    return obj;
}


Object3D* ObjectManager::getObject(const std::string& name)
{
	for(auto objs : mObjectsDefered)
	{
        for(Object3D* obj : objs.second)
            if (obj->getName() == name)
                return obj;
	}
    Gum::Output::error("ObjectManager: Texture Object3D " + name + " doesn't exist!");
    return nullptr;
}

void ObjectManager::iterateThroughObjects(std::function<void(Object3D* obj, ShaderProgram* shader, bool defered)> callback)
{
	for(auto objs : mObjectsDefered)
        for(Object3D* obj : objs.second)
            callback(obj, objs.first, true);
        
	for(auto objs : mObjectsForward)
        for(Object3D* obj : objs.second)
            callback(obj, objs.first, false);
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
	for(auto objs : mObjectsDefered)
	{
        for(Object3D* obj : objs.second)
        {
            for(unsigned int i = 0; i < obj->numInstances(); i++)
            {
                if (obj->getInstance(i)->getID() == id)
                {
                    return obj->getInstance(i);
                }
            }
        }
	}
	for(auto objs : mObjectsForward)
	{
        for(Object3D* obj : objs.second)
        {
            for(unsigned int i = 0; i < obj->numInstances(); i++)
            {
                if (obj->getInstance(i)->getID() == id)
                {
                    return obj->getInstance(i);
                }
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
	vec2 pos = Gum::Window::CurrentlyBoundWindow->getMouse()->getPosition();
	pos.y = renderer->getRenderCanvas()->getSize().y - pos.y;
    color col = renderer->getGBuffer()->getFramebuffer()->getPixel(pos);

	int pickedID = col.r + col.g * 256 + col.b * 256*256;
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
unsigned int ObjectManager::numObjects() 					  { return this->mObjectsDefered.size() + this->mObjectsForward.size(); }