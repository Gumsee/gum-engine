#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 TextureCoords;
layout (location = 2) in vec3 Normals;
layout (location = 3) in mat4 TransMatrix;

out vec2 TexCoords;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    mat4 FinalTrans = mat4(1);//transformationMatrix;
    //if(transformationMatrix == mat4(1))
        //FinalTrans = TransMatrix;
    
    vec4 worldPosition = FinalTrans * vec4(vertexPosition, 1.0f);
    vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;
    TexCoords = TextureCoords;

    gl_Position = positionRelativeToCam;
}