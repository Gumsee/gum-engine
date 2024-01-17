#pragma once
#include <Graphics/Shader.h>

static const std::string BoneRendererVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertices;
    layout (location = 3) in mat4 transMatrices;

	uniform mat4 viewMatrix;
	uniform mat4 projectionMatrix;

	void main()
    {
	    gl_Position = projectionMatrix * viewMatrix * transMatrices * vec4(vertices, 1.0);
	}
)";

static const std::string BoneRendererFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec3 Texcoord;
    out vec4 FragColor;
	
	void main() 
	{
        FragColor = vec4(1,1,0,1);
	}
)";