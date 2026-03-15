#pragma once
#include <Graphics/Shader.h>


static const std::string PostProcessingVertexShader = GLSL(
    in vec2 vertexPosition;
    out vec2 Texcoord;
    out mat4 projection;
    out mat4 view;

    uniform mat4 transformationMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;


    void main()
    {
        view = viewMatrix;
        projection = projectionMatrix;
        gl_Position = vec4(vertexPosition * 2.0 - vec2(1.0), 0.0f, 1.0f);
        Texcoord = vertexPosition;// ( + vec2(1.0)) / 2.0;
    }
);

static const std::string PostProcessingFragmentShader = GLSL(
    in vec2 Texcoord;
    out vec4 FragColor;

    uniform sampler2D textureSampler;

    void main(void)
    {
        FragColor = texture(textureSampler, Texcoord);
    }
);