#pragma once
#include <Graphics/Shader.h>


static const std::string OutlineRendererVertexShader = GLSL(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 3) in mat4 TransMatrix;

	uniform mat4 viewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 transformationMatrix;

	void main()
	{
		gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(vertexPosition, 1.0f);
	}
);

static const std::string OutlineRendererFragmentShader = GLSL(
    out float FragColor;

	void main()
	{
		FragColor = 1;
	}
);

static const std::string OutlineRendererProcessingFragmentShader = GLSL(
	in vec2 Texcoord;
    out vec4 FragColor;
    uniform sampler2D textureSampler;
    uniform vec4 color;

	void main()
	{
		bool hasWhite = false;
		bool hasBlack = false;
		
		for (int i = -2; i < 2; i+=2) 
		{
			for (int j = -2; j < 2; j+=2) 
			{
				float texsample = texelFetch(textureSampler, ivec2(gl_FragCoord) + ivec2(i, j), 0).r;
				if(texsample == 0.0)
				{
					hasBlack = true;
				}
				if(texsample == 1.0)
				{
					hasWhite = true;
				}
			}
		}

		if(hasWhite && hasBlack)
			FragColor = color;
		else
			FragColor = vec4(0,0,0,0);
	}
);