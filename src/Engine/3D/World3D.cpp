#include "World3D.h"
#include "../Shaders/ShaderManager.h"
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include <string>
#include "Camera3D.h"
#include "Codecs/XMLNode.h"
#include "Codecs/XMLWriter.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderProgram.h"
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
    World::update();
}

void World3D::renderSky()
{
    pObjectManager->getSkybox()->getShaderProgram()->use();
    pObjectManager->getSkybox()->render();
    pObjectManager->getSkybox()->getShaderProgram()->unuse();
}

void World3D::saveToFile(const Gum::Filesystem::File& file)
{
    XMLNode* rootNode = new XMLNode("world");

    //
    // Adding Meshes
    //
    /*std::vector<Mesh*> meshes;
    XMLNode* meshesNode = new XMLNode("meshes");
    for(unsigned int i = 0; i < getObjectManager()->numObjects(); i++)
    {
        Object3D* obj = getObjectManager()->getObject(i);
        Mesh* mesh = obj->getMesh();

        if(std::find(meshes.begin(), meshes.end(), mesh) == meshes.end())
        {
            XMLNode* meshNode = new XMLNode("mesh");
            meshNode->addAttribute("name", mesh->name);
            meshNode->addAttribute("id", std::to_string(meshes.size()));

            
            XMLNode* indicesNode = new XMLNode("indices");
            indicesNode->type = XMLNode::TEXT;
            for(unsigned int i = 0; i < mesh->numIndices(); i++)
                indicesNode->content += std::to_string(mesh->getIndex(i)) + " ";
            meshNode->addChild(indicesNode);
            
            XMLNode* verticesNode = new XMLNode("vertices");
            for(unsigned int j = 0; j < mesh->numVertices(); j++)
            {
                Vertex &vertex = mesh->getVertex(j);
                XMLNode* vertexNode = new XMLNode("vertex");
                vertexNode->addAttribute("p", vertex.position.toString("", "", ","));
                vertexNode->addAttribute("n", vertex.normal.toString("", "", ","));
                vertexNode->addAttribute("t", vertex.textureCoord.toString("", "", ","));
                vertexNode->addAttribute("j", vertex.JointIDs.toString("", "", ","));
                vertexNode->addAttribute("a", vertex.tangent.toString("", "", ","));
                vertexNode->addAttribute("b", vertex.bitangent.toString("", "", ","));
                vertexNode->addAttribute("w", vertex.Weights.toString("", "", ","));

                verticesNode->addChild(vertexNode);
            }
            meshNode->addChild(verticesNode);


        
            meshes.push_back(mesh);

            meshesNode->addChild(meshNode);
        }
    }
    rootNode->addChild(meshesNode);*/

    //
    // Adding Objects
    //
    XMLNode* objectsNode = new XMLNode("objects");
    getObjectManager()->iterateThroughObjects([objectsNode](Object3D* obj, ShaderProgram* shader, bool defered) {
        XMLNode* objNode = new XMLNode("object");
        objNode->addAttribute("name", obj->getName());
        objNode->addAttribute("shader", shader->getName());
        objNode->addAttribute("isdefered", defered ? "true" : "false");

        //int meshid = std::find(meshes.begin(), meshes.end(), obj->getMesh()) - meshes.begin();
        //objNode->addAttribute("meshid", std::to_string(meshid));
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
    });
    rootNode->addChild(objectsNode);


    //
    // Adding Lights
    //
    XMLNode* lightsNode = new XMLNode("lights");
    for(unsigned int i = 0; i < getLightManager()->numPointLights(); i++)
    {
        PointLight* light = getLightManager()->getPointLight(i);
        XMLNode* lightNode = new XMLNode("light");
        lightNode->addAttribute("name", light->getName());
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