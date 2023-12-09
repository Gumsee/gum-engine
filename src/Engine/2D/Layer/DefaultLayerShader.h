#pragma once
#include "../../Shaders/ShaderManager.h"

static const std::string DefaultLayerVertexShader = Shader::SHADER_VERSION_STR + 
R"(
layout (location = 0) in vec2 vertices;

void main()
{
	gl_Position = vec4(vertices, 0.0, 1.0);
}
)";

static const std::string DefaultLayerFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
uniform vec4 color = vec4(1,0,0,1);

out vec4 outColor;

void main()
{
    outColor = color;
}
)";

static ShaderProgram* initDefaultLayerShader()
{
    if(!Gum::ShaderManager::hasShaderProgram("DefaultLayerShader"))
    {
        ShaderProgram* shader;
        shader = new ShaderProgram(true);
        shader->addShader(new Shader(DefaultLayerVertexShader, Shader::TYPES::VERTEX_SHADER));
        shader->addShader(new Shader(DefaultLayerFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

        shader->build("DefaultLayerShader", { {"vertices", 0} });
        shader->addUniform("color");

        Gum::ShaderManager::addShaderProgram(shader);
        return shader;
    }
    
    
    return Gum::ShaderManager::getShaderProgram("DefaultLayerShader");
}