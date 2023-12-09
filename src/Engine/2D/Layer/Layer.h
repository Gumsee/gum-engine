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

public:
	Layer(std::string name, std::string type);
	virtual ~Layer() {};

	virtual void render() override {};

    //Getter
    std::string getName()                 { return this->sName; }
    std::string getType()                 { return this->sType; }
    ShaderProgram* getShader()            { return this->pShader; }

    //Setter
    void setName(std::string name)        { sName = name; }
    void setShader(ShaderProgram* shader) { this->pShader = shader; }
};