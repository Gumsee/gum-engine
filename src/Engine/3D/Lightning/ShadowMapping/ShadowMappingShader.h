#pragma once
#include <OpenGL/Shader.h>

static const std::string ShadowMappingVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 1) in vec2 TextureCoords;
    layout (location = 2) in vec3 Normals;
    layout (location = 3) in mat4 TransMatrix;

    out vec2 TexCoords;

    uniform mat4 transformationMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;
    uniform sampler2D Displacement;
    uniform int TextureMultiplier;
    uniform int isInstanced;

    void main()
    {
        mat4 FinalTrans = transformationMatrix;
        if(isInstanced == 1)
            FinalTrans = TransMatrix;

        vec3 FinalVertexPosition;
        vec4 DisplacementMap = texture(Displacement, TextureCoords * TextureMultiplier);
        float disp = 0.30*DisplacementMap.x + 0.59*DisplacementMap.y + 0.11*DisplacementMap.z;
        FinalVertexPosition = vec3(Normals * disp * 0.2) + vertexPosition;

        vec4 worldPosition = FinalTrans * vec4(vertexPosition, 1.0f);
        vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;
        TexCoords = TextureCoords * TextureMultiplier;

        gl_Position = positionRelativeToCam;
    }
)";

static const std::string ShadowMappingFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 TexCoords;
    out vec4 FragColor;
    uniform sampler2D texture;

    void main(void)
    {
        //float alpha = texture(texture, TexCoords).a;
        //if(alpha < 0.5) { discard; };
        FragColor = vec4(gl_FragCoord.z);
    }
)";