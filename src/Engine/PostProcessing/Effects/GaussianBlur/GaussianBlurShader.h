#pragma once
#include <Graphics/Shader.h>

static const std::string GussianBlurHShaderVert = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 vertexPosition;

    out vec2 blurTextureCoords[11];

    uniform float specialVar;

    void main()
    {
        gl_Position = vec4(vertexPosition * 2.0 - vec2(1.0), 0.0f, 1.0f);
        float pixelsize = 1.0f / specialVar;
        vec2 CenterTexCoords = vertexPosition;

        for(int i = -5; i <= 5; i++)
            blurTextureCoords[i + 5] = CenterTexCoords + vec2(0.0f,pixelsize * i);
    }
)";

static const std::string GussianBlurVShaderVert = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 vertexPosition;

    out vec2 blurTextureCoords[11];

    uniform float specialVar;

    void main()
    {
        gl_Position = vec4(vertexPosition * 2.0 - vec2(1.0), 0.0f, 1.0f);
        float pixelsize = 1.0f / specialVar;
        vec2 CenterTexCoords = vertexPosition;

        for(int i = -5; i <= 5; i++)
            blurTextureCoords[i + 5] = CenterTexCoords + vec2(pixelsize * i, 0.0f);
    }
)";




static const std::string GussianBlurShaderFrag = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 blurTextureCoords[11];

    out vec4 FragColor;

    uniform sampler2D textureSampler;

    void main()
    {
        FragColor = vec4(0.0);
        FragColor += texture(textureSampler, blurTextureCoords[0]) * 0.0093;
        FragColor += texture(textureSampler, blurTextureCoords[1]) * 0.028002;
        FragColor += texture(textureSampler, blurTextureCoords[2]) * 0.065984;
        FragColor += texture(textureSampler, blurTextureCoords[3]) * 0.121703;
        FragColor += texture(textureSampler, blurTextureCoords[4]) * 0.175713;
        FragColor += texture(textureSampler, blurTextureCoords[5]) * 0.198596;
        FragColor += texture(textureSampler, blurTextureCoords[6]) * 0.175713;
        FragColor += texture(textureSampler, blurTextureCoords[7]) * 0.121703;
        FragColor += texture(textureSampler, blurTextureCoords[8]) * 0.065984;
        FragColor += texture(textureSampler, blurTextureCoords[9]) * 0.028002;
        FragColor += texture(textureSampler, blurTextureCoords[10]) * 0.0093;
    }
)";