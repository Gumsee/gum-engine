#pragma once
#include "../Shaders/ShaderManager.h"

static const std::string SimpleVertexShader = Shader::SHADER_VERSION_STR + 
R"(
layout (location = 0) in vec3 vertices;
layout (location = 3) in mat4 TransMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * TransMatrix * vec4(vertices, 1.0);
}
)";

static const std::string SimpleFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
uniform vec4 color = vec4(1,0,0,1);

out vec4 finalColor;

void main()
{
    finalColor = color;
}
)";

static void initSimpleShader()
{
    ShaderProgram* shader;
    shader = new ShaderProgram(true);
    shader->addShader(new Shader(SimpleVertexShader, Shader::TYPES::VERTEX_SHADER));
    shader->addShader(new Shader(SimpleFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

    shader->build("SimpleShader", { {"vertices", 0}, {"TransMatrix", 3} });
    shader->addUniform("color");

    Gum::ShaderManager::addShaderProgram(shader);
}