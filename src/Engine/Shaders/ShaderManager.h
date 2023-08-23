#pragma once
#include <Graphics/ShaderProgram.h>

namespace Gum {
namespace ShaderManager
{
	extern ShaderProgram* addShaderProgram(ShaderProgram* shader, std::string Identifier = "");
    extern Shader* addShader(Shader* shader, std::string Identifier);

	/*
	* This function gets called by the Engine never call it yourself
	*/
	extern void cleanup();

    //Getter
    extern bool hasShaderProgram(std::string name);
	extern int numShaderPrograms();
    extern ShaderProgram* getShaderProgram(std::string name);
    extern Shader* getShader(std::string name);
}}