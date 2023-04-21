#pragma once
#include <OpenGL/Shader.h>


static const std::string PostProcessingVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 vertexPosition;
    out vec2 Texcoord;
    out mat4 projection;
    out mat4 viewMatrix;

    uniform mat4 transformationMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 viewmat;


    void main()
    {
        viewMatrix = viewmat;
        projection = projectionMatrix;
        gl_Position = vec4(vertexPosition * 2.0 - vec2(1.0), 0.0f, 1.0f);
        Texcoord = vertexPosition;// ( + vec2(1.0)) / 2.0;
    }
)";

static const std::string PostProcessingFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 Texcoord;
    in vec3 surfaceNormal;
    in vec3 toLightVector;

    uniform sampler2D textureSampler;
    uniform vec3 Color;

    const float contrast = 0.3;

    void main(void)
    {
        vec4 TextureColor = texture2D(textureSampler, Texcoord).rgba;
        gl_FragColor = TextureColor;
    }
)";