#include "World2D.h"
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include "Camera2D.h"
#include "Graphics/ShaderProgram.h"
#include "Layer/TileMap.h"
#include <Codecs/XMLWriter.h>
#include "Layer/Background.h"
#include "Layer/TileMap.h"

World2D::World2D()
    : World(WORLD2D)
{
    pLightManager = new LightManager(this);
    pPhysics = new Physics();
    //vCamera.push_back(new Camera(context->getSize(), this, context));
}

World2D::~World2D()
{
    Gum::_delete(pLightManager);
    Gum::_delete(pPhysics);
    for(size_t i = 0; i < vCamera.size(); i++)
        Gum::_delete(vCamera[i]);
}

void World2D::update()
{
    pLightManager->update();
    pPhysics->update();
}

void World2D::renderSky()
{
}

void World2D::saveToFile(const Gum::Filesystem::File& file)
{
    XMLNode* rootNode = new XMLNode("world");
    rootNode->addAttribute("type", "2D");
    rootNode->addAttribute("name", getName());

    //
    // Adding Objects
    //
    XMLNode* layersNode = new XMLNode("layers");
    for(unsigned int i = 0; i < numLayers(); i++)
    {
        Layer* layer = getSpriteLayer(i);
        XMLNode* layerNode = new XMLNode("layer");
        layerNode->addAttribute("name", layer->getName());
        layerNode->addAttribute("type", layer->getType());
        layerNode->addAttribute("shader", layer->getShader()->getName());
        
        layersNode->addChild(layerNode);
    }
    rootNode->addChild(layersNode);

    XMLWriter writer(file, rootNode);
}

World2D* World2D::readFromFile(XMLReader& reader)
{
    World2D* world = new World2D();
    XMLNode* rootNode = reader.getRootNode();
    world->setName(rootNode->getAttribute("name").toString());

    XMLNode* layersNode = rootNode->findChild("layers");
    if(layersNode != nullptr)
    {
        for(XMLNode* layernode : layersNode->children)
        {
            std::string type = layernode->getAttribute("type").toString();
            std::string name = layernode->getAttribute("name").toString();
            Layer* layer = nullptr;
            if     (type == "background") layer = new Background(name);
            else if(type == "tilemap")    layer = new TileMap(name);

            ShaderProgram* shader = ShaderProgram::getShaderProgramByName(layernode->getAttribute("shader").toString());

            if(layer != nullptr)
            {
                if(shader != nullptr)
                    layer->setShader(shader);
                world->addLayer(layer);
            }
        }
    }

    return world;
}




void World2D::addLayer(Layer* layer) { vLayers.push_back(layer); }

//
// Getter
//
LightManager* World2D::getLightManager()                  { return this->pLightManager; }
unsigned int World2D::numLayers() const                   { return this->vLayers.size(); }
Layer* World2D::getSpriteLayer(const unsigned int& layer) { return this->vLayers[layer]; }