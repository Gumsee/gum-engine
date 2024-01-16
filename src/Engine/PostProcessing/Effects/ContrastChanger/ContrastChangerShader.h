#pragma once
#include <Graphics/Shader.h>

static const std::string ContrastChangerFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 Texcoord;

    out vec4 FragColor;

    uniform sampler2D texture0;
    uniform float contrast; // [0.0; 1.0]

    void main(void)
    {
        FragColor = texture(texture0, Texcoord);
        FragColor.rgb = (FragColor.rgb - 0.5) * (1.0 + contrast) + 0.5;
    }
)";