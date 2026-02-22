#include "Layer.h"
#include "DefaultLayerShader.h"

Layer::Layer(std::string name, std::string type) : sName(name), sType(type)
{
    pShader = initDefaultLayerShader();
}


//Getter
std::string Layer::getName()                 { return this->sName; }
std::string Layer::getType()                 { return this->sType; }
ShaderProgram* Layer::getShader()            { return this->pShader; }

//Setter
void Layer::setName(std::string name)        { sName = name; }
void Layer::setShader(ShaderProgram* shader) { this->pShader = shader; }

void Layer::setTexture(Texture *tex, int index)
{
    if(tex == nullptr)
    {
		delTexture(index);
        return;
    }

	mTextures[index] = tex;
}

void Layer::delTexture(int index)
{
	mTextures.erase(mTextures.find(index));
}

void Layer::bindTextures()
{
	for (auto it : mTextures)
        it.second->bind(it.first);
}
void Layer::unbindTextures()
{
	for (auto it : mTextures)
        it.second->unbind(it.first);
}