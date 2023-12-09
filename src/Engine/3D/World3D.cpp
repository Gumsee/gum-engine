#include "World3D.h"
#include "../Shaders/ShaderManager.h"
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include "Camera3D.h"
#include "Codecs/XMLWriter.h"
#include "Object/PhysicsObjectInstance.h"


World3D::World3D()
    : World(WORLD3D)
{
    pLightManager = new LightManager(this); 
    pObjectManager = new ObjectManager(pLightManager->getSun()->getDirection()); 
    pPhysics = new Physics();
    //vCamera.push_back(new Camera(context->getSize(), this, context));
}

World3D::~World3D()
{
    Gum::_delete(pLightManager);
    Gum::_delete(pObjectManager);
    Gum::_delete(pPhysics);
    for(size_t i = 0; i < vCamera.size(); i++)
        Gum::_delete(vCamera[i]);
}

void World3D::update()
{
    pLightManager->update();
    pPhysics->update();
}

void World3D::renderSky()
{
    pObjectManager->getSkybox()->getShaderProgram()->use();
    pObjectManager->getSkybox()->getShaderProgram()->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pObjectManager->getSkybox()->getShaderProgram()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pObjectManager->getSkybox()->render();
    pObjectManager->getSkybox()->getShaderProgram()->unuse();
}

void World3D::saveToFile(const Gum::Filesystem::File& file)
{
    XMLNode* rootNode = new XMLNode("world");

    //
    // Adding Objects
    //
    XMLNode* objectsNode = new XMLNode("objects");
    for(unsigned int i = 0; i < getObjectManager()->numObjects(); i++)
    {
        Object3D* obj = getObjectManager()->getObject(i);
        XMLNode* objNode = new XMLNode(obj->getName());
        for(unsigned int j = 0; j < obj->numInstances(); j++)
        {
            Object3DInstance* inst = obj->getInstance(j);
            XMLNode* instanceNode = new XMLNode("instance");
            instanceNode->addAttribute("id", std::to_string(j));
            instanceNode->addAttribute("position", inst->getPosition().toString());
            instanceNode->addAttribute("rotation", inst->getRotation().toString());
            instanceNode->addAttribute("scale", inst->getScale().toString());
            instanceNode->addAttribute("physics", inst->getType() == OBJECT3D_INSTANCE_TYPE_PHYSICS ? "true" : "false");
            objNode->addChild(instanceNode);
        }
        objectsNode->addChild(objNode);
    }
    rootNode->addChild(objectsNode);


    //
    // Adding Lights
    //
    XMLNode* lightsNode = new XMLNode("lights");
    for(unsigned int i = 0; i < getLightManager()->numPointLights(); i++)
    {
        PointLight* light = getLightManager()->getPointLight(i);
        XMLNode* lightNode = new XMLNode(light->getName());
        lightNode->addAttribute("position", light->getPosition().toString());
        lightNode->addAttribute("color", light->getColor().toString());
        lightNode->addAttribute("strength", std::to_string(light->getStrength()));
        lightNode->addAttribute("attenuation", light->getAttenuation().toString());

        lightsNode->addChild(lightNode);
    }
    rootNode->addChild(lightsNode);

    XMLWriter writer(file, rootNode);
}

World3D* World3D::readFromFile(XMLReader& reader)
{
    World3D* world = new World3D();
     
    for(XMLNode* objsHolderNode : reader.getNodeListByName("objects"))
    {
        for(XMLNode* objNode : objsHolderNode->children)
        {
            Object3D* obj = new Object3D(Mesh::generateCube(vec3(1)), objNode->name); //TODO
            for(XMLNode* instNode : objNode->children)
            {
                Object3DInstance* inst = obj->addInstance();
                inst->setPosition(instNode->getVec3Attribute("position"));
                inst->setRotation(instNode->getVec3Attribute("rotation"));
                inst->setScale(instNode->getVec3Attribute("scale"));
            }
            world->getObjectManager()->addObject(obj);
        }
    }

    return world;
}


//
// Getter
//
LightManager* World3D::getLightManager()   { return this->pLightManager; }
ObjectManager* World3D::getObjectManager() { return this->pObjectManager; }