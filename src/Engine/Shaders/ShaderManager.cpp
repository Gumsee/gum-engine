#include "ShaderManager.h"
#include "Essentials/Tools.h"
#include <System/MemoryManagement.h>
#include <Graphics/ShaderProgram.h>
#include <System/Output.h>
#include <map>

namespace Gum {
namespace ShaderManager
{
	std::vector<ShaderProgram*> mShaderPrograms;
	std::vector<Shader*> mShaders;

	unsigned int addShaderProgram(ShaderProgram* shader)
	{
		if(shader == nullptr)
			Gum::Output::error("ShaderManager: Trying to add nullptr shaderprogram!");

		mShaderPrograms.push_back(shader);
		return mShaderPrograms.size() - 1;
	}

	unsigned int addShader(Shader* shader)
	{
		if(shader == nullptr)
			Gum::Output::error("ShaderManager: Trying to add nullptr shader!");
		
		mShaders.push_back(shader);
		return mShaders.size() - 1;
	}


	void cleanup()
	{
		for(ShaderProgram* shader : mShaderPrograms)
			Gum::_delete(shader);
		mShaders.clear();
		mShaderPrograms.clear();
	}


	//
	// Getter
	//
	unsigned int numShaderPrograms()                 { return mShaderPrograms.size(); }
	bool hasShaderProgram(std::string name) 
    {
		for(ShaderProgram* shader : mShaderPrograms)
			if(shader->getName() == name)
                return true;

        return false;
    }

	ShaderProgram* getShaderProgram(std::string name) 
	{ 
		for(ShaderProgram* shader : mShaderPrograms)
			if(shader->getName() == name)
                return shader;

		return nullptr; 
	}

	ShaderProgram* getShaderProgram(const unsigned int& index) 
	{
        if(index >= mShaderPrograms.size())
            return nullptr;

		return mShaderPrograms[index]; 
	}

	Shader* getShader(const unsigned int& index)
	{
        if(index >= mShaders.size())
            return nullptr;

		return mShaders[index];
	}
}}