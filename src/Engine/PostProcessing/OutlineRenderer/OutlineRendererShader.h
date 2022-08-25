#pragma once
#include <OpenGL/Shader.h>


static const std::string OutlineRendererVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 2) in vec3 Normals;

	uniform mat4 viewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 transformationMatrix;

    const float scale = 0.008f;

	void main()
	{
        vec4 worldPosition = transformationMatrix * vec4(vertexPosition, 1.0f);
        vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;
		gl_Position = positionRelativeToCam;
	}
)";

static const std::string OutlineRendererFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	void main()
	{
		vec4 color = vec4(1,1,1,1);
		gl_FragColor = color;
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
				vec3 sample = texelFetch(textureSampler, ivec2(gl_FragCoord) + ivec2(i, j), 0).rgb;
				if(sample == vec3(0,0,0))
				{
					hasBlack = true;
				}
				if(sample == vec3(1,1,1))
				{
					hasWhite = true;
				}
			}
		}

		if(hasWhite && hasBlack)
			gl_FragColor = vec4(0.36, 0.38, 0.64, 1.0);
		else
			gl_FragColor = vec4(0,0,0, 0.0);
	}
)";