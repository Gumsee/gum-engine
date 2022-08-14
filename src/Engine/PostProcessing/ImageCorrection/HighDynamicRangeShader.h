#pragma once
#include <OpenGL/Shader.h>

static const std::string HighDynamicRangeFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec2 vary_Texcoord;

    uniform float exposure;
    uniform sampler2D texture;

    void main()
    {             
        const float gamma = 2.2;
        vec3 hdrColor = texture2D(texture, vary_Texcoord).rgb;
    
        // exposure tone mapping
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
        // gamma correction 
        mapped = pow(mapped, vec3(1.0 / gamma));
    
        gl_FragColor = vec4(mapped, 1.0);
    } 
)";