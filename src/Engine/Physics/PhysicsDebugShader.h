#pragma once
#include <OpenGL/Shader.h>

static const std::string PhysicsDebuggingVertexShader = Shader::SHADER_VERSION_STR + R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 1) in vec2 TextureCoords;
    layout (location = 2) in vec3 Normals;
    layout (location = 3) in mat4 TransMatrix;

    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0f);
    }
)";

static const std::string PhysicsDebuggingFragmentShader = Shader::SHADER_VERSION_STR + R"(
    out vec4 FragColor;
    uniform vec4 color;

    void main(void)
    {
        FragColor = color;
        //FragColor = vec4(1,0,0,1);
    }
)";