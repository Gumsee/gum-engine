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
        vec4 worldPosition = transformationMatrix * vec4(vertexPosition + Normals * scale, 1.0f);
        vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;
		gl_Position = positionRelativeToCam;
	}
)";

static const std::string OutlineRendererFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	void main()
	{
		vec4 color = vec4(1,0,1,1);
		gl_FragColor = color;
	}
)";