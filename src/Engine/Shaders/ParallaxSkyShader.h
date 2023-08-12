#pragma once
#include <OpenGL/Shader.h>

static const std::string ParallaxSkyVertexShader = Shader::SHADER_VERSION_STR + 
R"(
layout (location = 0) in vec2 vertices;

out vec2 Texcoord;

uniform mat4 viewMatrix;

void main()
{
    gl_Position = vec4(vertices, 0.0f, 1.0f);
    Texcoord = vertices;
    //if(!invertY) { Texcoord.y = 1 - Texcoord.y; }
}
)";

static const std::string ParallaxSkyFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
in vec2 Texcoord;
uniform sampler2D textureSampler;

out vec4 fragColor;


void main(void)
{
    vec4 TextureColor, color;
    TextureColor = texture(textureSampler, Texcoord);
    fragColor = TextureColor * color; 
    fragColor = vec4(1,0,0,1);
}
)";