#pragma once
#include <OpenGL/Shader.h>


static const std::string BillboardVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;

    out vec2 Texcoord;
    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 transformationMatrix;
    uniform vec3 billboardCenter;
    uniform vec2 billboardSize;
    uniform int fixedSize;

    void main()
    {
        vec4 positionRelativeToCam;
        if(fixedSize > 0) //Fixed Size Billboard
        {
            gl_Position = projectionMatrix * viewMatrix * vec4(billboardCenter, 1.0f);
            gl_Position /= gl_Position.w;
            gl_Position.xy += vertexPosition.xy * billboardSize;
        }
        else //3D Billboard
        {
            vec3 CameraRight_worldspace = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
            vec3 CameraUp_worldspace = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
            vec3 vertexPosition_worldspace = billboardCenter + CameraRight_worldspace * vertexPosition.x * billboardSize.x + CameraUp_worldspace * vertexPosition.y * billboardSize.y;
            gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition_worldspace, 1.0f);
        }
        Texcoord = vec2(vertexPosition.x, vertexPosition.y * -1) + vec2(0.5,0.5);
    }
)";

static const std::string BillboardFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 Texcoord;
    out vec4 FragColor;

    uniform sampler2D textureSampler;

    void main(void)
    {
        vec4 color = texture(textureSampler, Texcoord);
        FragColor = color;
    }
)";