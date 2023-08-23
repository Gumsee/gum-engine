#include "ShaderManager.h"
#include "Essentials/Tools.h"
#include <System/MemoryManagement.h>
#include <Graphics/ShaderProgram.h>
#include <System/Output.h>
#include <map>

namespace Gum {
namespace ShaderManager
{
	std::map<std::string, ShaderProgram*> mShaderPrograms;
	std::map<std::string, Shader*> mShaders;

	ShaderProgram* addShaderProgram(ShaderProgram* shader, std::string Identifier)
	{
		if(shader == nullptr)
			Gum::Output::error("ShaderManager: Trying to add nullptr shaderprogram! (" + Identifier + ")");
		if(Identifier == "") { Identifier = shader->getName(); }
		mShaderPrograms[Identifier] = shader;
		Gum::Output::debug("ShaderManager: Added Shaderprogram, " + Identifier);

		return shader;
	}

	Shader* addShader(Shader* shader, std::string Identifier)
	{
		if(shader == nullptr)
			Gum::Output::error("ShaderManager: Trying to add nullptr shader! (" + Identifier + ")");
		
		mShaders[Identifier] = shader;
		Gum::Output::debug("ShaderManager: Added Shader, " + Identifier);

		return shader;
	}


	void cleanup()
	{
		for(auto program : mShaderPrograms)
		{
			std::cout << "cleaning " << program.first << std::endl;
			Gum::_delete(program.second);
		}
		mShaders.clear();
	}


	//
	// Getter
	//
	int numShaderPrograms()                 { return mShaderPrograms.size(); }
	bool hasShaderProgram(std::string name) { return Tools::mapHasKeyNotNull(mShaderPrograms, name); }

	ShaderProgram* getShaderProgram(std::string name) 
	{ 
		if(!Tools::mapHasKeyNotNull(mShaderPrograms, name))
			Gum::Output::error("Shaderprogram " + name + " does not exist!");

		return mShaderPrograms[name]; 
	}

	Shader* getShader(std::string name)
	{
		if(!Tools::mapHasKeyNotNull(mShaders, name))
			Gum::Output::error("Shader " + name + " does not exist!");
		return mShaders[name];
	}
}}