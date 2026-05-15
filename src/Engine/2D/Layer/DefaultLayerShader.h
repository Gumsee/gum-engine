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
  ShaderProgram* shader;
  shader = ShaderProgram::requestShaderProgram("DefaultLayerShader", true);
  shader->addShader(Shader::requestShader("DefaultLayerShader", DefaultLayerVertexShader, Shader::TYPES::VERTEX_SHADER));
  shader->addShader(Shader::requestShader("DefaultLayerShader", DefaultLayerFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

  shader->build({ {"vertices", 0} });
  return shader;
}