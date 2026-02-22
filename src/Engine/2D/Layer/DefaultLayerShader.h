#pragma once
#include <Graphics/ShaderProgram.h>

static const std::string DefaultLayerVertexShader = GLSL(
    layout (location = 0) in vec2 vertices;

    void main()
    {
        gl_Position = vec4(vertices, 0.0, 1.0);
    }
);

static const std::string DefaultLayerFragmentShader = GLSL(
    uniform vec4 color = vec4(1,0,0,1);

    out vec4 outColor;

    void main()
    {
        outColor = color;
    }
);

static ShaderProgram* initDefaultLayerShader()
{
    if(ShaderProgram::getShaderProgramByName("DefaultLayerShader") == nullptr)
    {
        ShaderProgram* shader;
        shader = new ShaderProgram("DefaultLayerShader", true);
        shader->addShader(new Shader(DefaultLayerVertexShader, Shader::TYPES::VERTEX_SHADER));
        shader->addShader(new Shader(DefaultLayerFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

        shader->build({ {"vertices", 0} });
        return shader;
    }
    
    
    return ShaderProgram::getShaderProgramByName("DefaultLayerShader");
}