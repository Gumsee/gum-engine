#pragma once
#include <Graphics/Shader.h>

static const std::string CombineTexturesFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 Texcoord;
    out vec4 FragColor;

	uniform sampler2D texture0;
	uniform sampler2D texture1;
	uniform float exposure;

	void main()
	{
		FragColor = texture(texture0, Texcoord) + texture(texture1, Texcoord) * exposure;
	}
)";