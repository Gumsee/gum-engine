#include "World3D.h"
#include <GumEngine/Engine/3D/Object/SceneObject.h>
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include <string>
#include "Camera3D.h"
#include "Codecs/XMLNode.h"
#include "Codecs/XMLWriter.h"
#include "Essentials/Unicode.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderProgram.h"
#include "Object/ObjectManager.h"
#include "../Material/MaterialManager.h"
#include "../Texture/TextureManager.h"
#include "Object/PhysicsObjectInstance.h"
#include "Primitives/Transformable.h"
#include "System/Output.h"


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
            if(inst->getPosition() != vec3(0,0,0))  instanceNode->addAttribute("position", inst->getPosition().toString());
            if((quat)inst->getRotation() != quat()) instanceNode->addAttribute("rotation", inst->getRotation().toString());
            if(inst->getScale() != vec3(1,1,1))     instanceNode->addAttribute("scale", inst->getScale().toString());
            if(inst->getType() == OBJECT3D_INSTANCE_TYPE_PHYSICS) instanceNode->addAttribute("physics",  "true");
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
    XMLNode* worldNode = reader.getRootNode();

    XMLNode *skynode = worldNode->findChild("sky");
    if(skynode != nullptr)
    {
        bool gradient = skynode->getAttribute("gradient") == "true";
        Gum::Unicode skytexture = skynode->getAttribute("texture");
        world->getObjectManager()->getSkybox()->useGradiant(gradient);
        world->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture(skytexture.toString(), true));
    }
     
    for(XMLNode* objsHolderNode : reader.getNodeListByName("objects"))
    {
        for(XMLNode* objNode : objsHolderNode->children)
        {
            Gum::Unicode objname = objNode->getAttribute("name");
            Gum::Unicode materialfile = objNode->getAttribute("material");
            Gum::Unicode meshfile = objNode->getAttribute("mesh");
            if(meshfile.isEmpty())
                continue;

            std::vector<std::string> meshstrs = Tools::splitStr(meshfile.toString(), ':');
            SceneObject* obj;
            if(meshstrs.size() == 3 && meshstrs[0] == "int")
            {
                if(meshstrs[1] == "plane")
                    obj = new SceneObject(Mesh::generatePlane(Tools::StringToVec2(meshstrs[2])), objname.toString());
            }
            else
            {
                obj = new SceneObject(ObjectManager::MODEL_ASSETS_PATH + meshfile.toString(), objname.toString());
            }

            if(!materialfile.isEmpty())
                obj->setMaterial(new Material(Gum::MaterialManager::MATERIAL_ASSETS_PATH + materialfile.toString()));
            for(XMLNode* instNode : objNode->children)
            {
                Object3DInstance* inst = obj->addInstance();
                inst->setPosition(instNode->getVec3Attribute("position"));
                inst->setRotation(instNode->getVec3Attribute("rotation"));
                inst->setScale(instNode->getVec3Attribute("scale", vec3(1,1,1)));
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