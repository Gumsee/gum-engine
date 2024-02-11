#pragma once
#include <Graphics/Shader.h>

static const std::string SpriteVertexShader = GLSL(
    layout (location = 0) in vec2 vertices;
    layout (location = 1) in mat3 TransMatrix;

    out vec2 Texcoord;

    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;

    void main()
    {
        vec3 finalpos = TransMatrix * vec3(vertices, -1.0f);
        gl_Position = projectionMatrix * viewMatrix * vec4(finalpos, 1.0f);
        Texcoord = vertices;
        //if(!invertY) { Texcoord.y = 1 - Texcoord.y; }
    }
);

static const std::string SpriteFragmentShader = GLSL(
    in vec2 Texcoord;
    uniform sampler2D textureSampler;

    out vec4 fragColor;


    void main(void)
    {
        vec4 TextureColor;
        TextureColor = texture(textureSampler, Texcoord);
        fragColor = TextureColor; 
        fragColor = vec4(1,0,0,1);
    }
);