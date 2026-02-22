#include "TileMap.h"
#include <System/MemoryManagement.h>
#include <string>
#include "../../Shaders/SpriteShader.h"
#include "../../Rendering/Camera.h"

TileMap::TileMap(std::string name)
    : Layer(name, "tilemap")
{
    initShader();
}

TileMap::~TileMap() 
{
	for(Sprite2D* spr : vSprites)
		Gum::_delete(spr);

	vSprites.clear();
}

void TileMap::render()
{
    pShader->use();
    for(Sprite2D* spr : vSprites)
        spr->render();
}


Sprite2D* TileMap::addSprite(Sprite2D *spr)
{
	vSprites.push_back(spr);
    if(pAddSpriteCallback != nullptr)
        pAddSpriteCallback(spr);

    return spr;
}

void TileMap::removeSprite(Sprite2D *sprToDelete)
{
	
}


//
// Setter
//
void TileMap::onAddSprite(AddSpriteCallback callback)   { this->pAddSpriteCallback = callback; }


//
//Getter
//
unsigned int TileMap::numSprites() 					  { return this->vSprites.size(); }
bool TileMap::hasSprite(const std::string& name)        { return getSprite(name) != nullptr; }
Sprite2D* TileMap::getSprite(const std::string& name)
{
	for(Sprite2D* spr : vSprites)
	{
		if (spr->getName() == name)
			return spr;
	}
    Gum::Output::error("TileMap: Texture Sprite2D " + name + " doesn't exist!");
    return nullptr;
}

Sprite2DInstance* TileMap::getInstanceByID(const unsigned int& id)
{
	for(Sprite2D* spr : vSprites)
	{
		for(unsigned int i = 0; i < spr->numInstances(); i++)
		{
			if (spr->getInstance(i)->getID() == id)
			{
				return spr->getInstance(i);
			}
		}
	}
    Gum::Output::warn("TileMap: Sprite2D with ID " + std::to_string(id) + " doesn't exist!");
    return nullptr;
}


void TileMap::initShader()
{
    if(ShaderProgram::getShaderProgramByName("SpriteShader") == nullptr)
    {
        pShader = new ShaderProgram("SpriteShader", true);
        pShader->addShader(new Shader(SpriteVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(SpriteFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

        pShader->build({{"vertices", 0}, {"TransMatrix", 1}});
        pShader->addTexture("textureSampler", 0);
    }

    pShader = ShaderProgram::getShaderProgramByName("SpriteShader");
}