#pragma once
#include <Graphics/Shader.h>

static const std::string BrightfilterFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 Texcoord;
    
    out vec4 FragColor;

    uniform sampler2D texture0;

    void main(void)
    {
        vec4 color = texture(texture0, Texcoord);
        float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
        FragColor = color * brightness;
    }
)";