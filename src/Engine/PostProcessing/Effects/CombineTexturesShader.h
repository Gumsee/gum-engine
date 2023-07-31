#pragma once
#include <OpenGL/Shader.h>


static const std::string CombineTexturesVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec3 vertexPosition;
    out vec2 Texcoord;

    void main()
    {
        gl_Position = vec4(vertexPosition, 1.0f);
        Texcoord = (vertexPosition.xy + vec2(1.0)) / 2.0;
    }
)";

static const std::string CombineTexturesFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 Texcoord;
    out vec4 FragColor;

	uniform sampler2D texture1;
	uniform sampler2D texture2;
	uniform float exposure;

	void main()
	{
		FragColor = texture(texture1, Texcoord) + texture(texture2, Texcoord);
	}
)";