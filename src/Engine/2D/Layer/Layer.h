#pragma once
#include "Graphics/ShaderProgram.h"
#include <Primitives/UniqueID.h>
#include <Essentials/Settings.h>
#include <functional>
#include <Graphics/Sprite2D.h>

class Layer : public Renderable, public UniqueID
{
protected:
	std::string sName;
	std::string sType;
    ShaderProgram* pShader;
	std::unordered_map<unsigned int, Texture*> mTextures;

public:
	Layer(std::string name, std::string type);
	virtual ~Layer() {};

	virtual void render() override {};

    //Getter
    std::string getName();
    std::string getType();
    ShaderProgram* getShader();

    //Setter
    void setName(std::string name);
    void setShader(ShaderProgram* shader);
	void setTexture(Texture *tex, int index);
	void delTexture(int index);
	void bindTextures();
	void unbindTextures();
};