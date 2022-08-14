#version 330 core
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 TextureCoords;
layout (location = 2) in vec3 Normals;
layout (location = 3) in mat4 TransMatrix;

out vec2 TexCoords;

uniform mat4 transformationMatrix;
uniform mat4 projectionViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main() 
{
    //Set the x,y position on the screen
	vec4 worldPosition = TransMatrix * vec4(vertexPosition, 1.0f);
	//vec4 positionRelativeToCam = projectionViewMatrix * worldPosition;
	vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;

	TexCoords = TextureCoords;

    gl_Position = positionRelativeToCam;
}