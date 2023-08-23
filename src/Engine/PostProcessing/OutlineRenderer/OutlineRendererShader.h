#pragma once
#include <Graphics/Shader.h>


static const std::string OutlineRendererVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 2) in vec3 Normals;

	uniform mat4 viewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 transformationMatrix;

	void main()
	{
		gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(vertexPosition, 1.0f);
	}
)";

static const std::string OutlineRendererFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1,1,1,1);
	}
)";


static const std::string OutlineRendererProcessingVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec3 vertexPosition;
    out vec2 Texcoord;

    void main()
    {
        gl_Position = vec4(vertexPosition, 1.0f);
        Texcoord = (vertexPosition.xy + vec2(1.0)) / 2.0;
    }
)";

static const std::string OutlineRendererProcessingFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec2 Texcoord;
    out vec4 FragColor;
    uniform sampler2D textureSampler;

	void main()
	{
		vec3 color;
		bool hasWhite = false;
		bool hasBlack = false;
		
		for (int i = -2; i < 2; i+=2) 
		{
			for (int j = -2; j < 2; j+=2) 
			{
				vec3 texsample = texelFetch(textureSampler, ivec2(gl_FragCoord) + ivec2(i, j), 0).rgb;
				if(texsample == vec3(0,0,0))
				{
					hasBlack = true;
				}
				if(texsample == vec3(1,1,1))
				{
					hasWhite = true;
				}
			}
		}

		if(hasWhite && hasBlack)
			FragColor = vec4(0.36, 0.38, 0.64, 1.0);
		else
			FragColor = vec4(0,0,0, 0.0);
	}
)";