#pragma once
#include <OpenGL/Shader.h>


static const std::string ParticleVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 1) in vec3 partPositions;
    layout (location = 2) in vec2 partSizes;
    layout (location = 3) in vec4 texOffset;
    layout (location = 4) in vec2 TexCoordInfo;

    out vec2 Texcoord1;
    out vec2 Texcoord2;
    out float blend;

    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;


    void main()
    {
        vec3 CameraRight_worldspace = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
        vec3 CameraUp_worldspace = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
        vec3 vertexPosition_worldspace = partPositions + CameraRight_worldspace * vertexPosition.x * partSizes.x + CameraUp_worldspace * vertexPosition.y * partSizes.y;
        gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition_worldspace, 1.0f);

        // vec2 Texcoord = vec2((vertexPosition.x+1.0)/2.0 * -1, 1 - (vertexPosition.y+1.0)/2.0);
        vec2 Texcoord = vertexPosition.xy + vec2(0.5);
        Texcoord.y = 1 - Texcoord.y;
        Texcoord /= TexCoordInfo.x;
        Texcoord1 = Texcoord + texOffset.xy;
        Texcoord2 = Texcoord + texOffset.zw;
        blend = TexCoordInfo.y;
    }
)";

static const std::string ParticleFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in float visibility;
    in vec2 Texcoord1;
    in vec2 Texcoord2;
    in float blend;
    out vec4 FragColor;

    uniform sampler2D textureSampler;

    void main(void)
    {
        vec4 color1 = texture(textureSampler, Texcoord1);
        vec4 color2 = texture(textureSampler, Texcoord2);
        vec4 finalColor = mix(color1, color2, blend);
        FragColor = finalColor;
        FragColor = vec4(Texcoord1,1,1);
        FragColor = color1;
    }
)";