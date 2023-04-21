// #include "WorldLoader.h"
// #include "Codecs/XMLNode.h"
// #include <Codecs/XMLWriter.h>
// #include <Codecs/XMLReader.h>
// #include <Essentials/Tools.h>
// #include <GumEssentials/Essentials/Tools.h>
// #include <string>


// World* WorldLoader::loadWorld(std::string filepath, std::string assetspath)
// {
//     World* world = new World();

//     XMLReader reader(filepath);
//     for(XMLNode* objsHolderNode : reader.getNodeListByName("objects"))
//     {
//         for(XMLNode* objNode : objsHolderNode->children)
//         {
//             Object* obj = new Object(Mesh::generateCube(vec3(1)), objNode->name); //TODO
//             for(XMLNode* instNode : objNode->children)
//             {
//                 Instance* inst = new Instance();
//                 inst->setPosition(Tools::StringToVec3(objNode->mAttributes["position"]));
//                 inst->setRotation(Tools::StringToVec3(objNode->mAttributes["rotation"]));
//                 inst->setScale(Tools::StringToVec3(objNode->mAttributes["scale"]));
//                 obj->addInstance(inst);
//             }
//             world->getObjectManager()->addObject(obj);
//         }
//     }

//     return world;
// }

// void WorldLoader::saveWorld(std::string filepath, World* world)
// {
//     XMLNode* rootNode = new XMLNode("world");

//     //
//     // Adding Objects
//     //
//     XMLNode* objectsNode = new XMLNode("objects");
//     for(unsigned int i = 0; i < world->getObjectManager()->numObjects(); i++)
//     {
//         Object* obj = world->getObjectManager()->getObject(i);
//         XMLNode* objNode = new XMLNode(obj->getName());
//         for(unsigned int j = 0; j < obj->numInstances(); j++)
//         {
//             Instance* inst = obj->getInstance(j);
//             XMLNode* instanceNode = new XMLNode("instance");
//             instanceNode->addAttribute("id", std::to_string(j));
//             instanceNode->addAttribute("position", inst->getPosition().toString());
//             instanceNode->addAttribute("rotation", inst->getRotation().toString());
//             instanceNode->addAttribute("scale", inst->getScale().toString());
//             instanceNode->addAttribute("physics", std::to_string(inst->getBody() != nullptr));
//             objNode->addChild(instanceNode);
//         }
//         objectsNode->addChild(objNode);
//     }
//     rootNode->addChild(objectsNode);


//     //
//     // Adding Lights
//     //
//     XMLNode* lightsNode = new XMLNode("lights");
//     for(unsigned int i = 0; i < world->getLightManager()->numPointLights(); i++)
//     {
//         PointLight* light = world->getLightManager()->getPointLight(i);
//         XMLNode* lightNode = new XMLNode(light->getName());
//         lightNode->addAttribute("position", light->getPosition().toString());
//         lightNode->addAttribute("color", light->getColor().toString());
//         lightNode->addAttribute("strength", std::to_string(light->getStrength()));
//         lightNode->addAttribute("attenuation", light->getAttenuation().toString());

//         lightsNode->addChild(lightNode);
//     }
//     rootNode->addChild(lightsNode);

//     XMLWriter writer(filepath, rootNode);
// }