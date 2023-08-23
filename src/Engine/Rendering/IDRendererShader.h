#pragma once
#include <Graphics/Shader.h>

static const std::string MeshIDVertexShader = Shader::SHADER_VERSION_STR + 
R"(
layout (location = 0) in vec3 vertices;
layout (location = 3) in mat4 TransMatrix;
layout (location = 10) in vec4 individualColor;

out vec4 individualColorFS;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    individualColorFS = individualColor;
	gl_Position = projectionMatrix * viewMatrix * TransMatrix * vec4(vertices, 1.0);
}
)";

static const std::string MeshIDFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
in vec4 individualColorFS;

out vec4 finalColor;

void main()
{
    finalColor = individualColorFS;
}
)";