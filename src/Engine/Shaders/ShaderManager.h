#pragma once
#include <Graphics/ShaderProgram.h>

namespace Gum {
namespace ShaderManager
{
	extern unsigned int addShaderProgram(ShaderProgram* shader);
    extern unsigned int addShader(Shader* shader);

	/*
	* This function gets called by the Engine never call it yourself
	*/
	extern void cleanup();

    //Getter
    extern bool hasShaderProgram(std::string name);
	extern unsigned int numShaderPrograms();
    extern ShaderProgram* getShaderProgram(std::string name);
    extern ShaderProgram* getShaderProgram(const unsigned int& index);
    extern Shader* getShader(const unsigned int& index);
}}