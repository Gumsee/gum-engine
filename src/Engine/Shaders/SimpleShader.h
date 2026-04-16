#pragma once
#include <Graphics/ShaderProgram.h>

static const std::string SimpleVertexShader = GLSL(
  layout (location = 0) in vec3 vertices;
  layout (location = 3) in mat4 TransMatrix;

  uniform mat4 projectionMatrix;
  uniform mat4 viewMatrix;

  void main()
  {
    gl_Position = projectionMatrix * viewMatrix * TransMatrix * vec4(vertices, 1.0);
  }
);

static const std::string SimpleFragmentShader = GLSL(
  uniform vec4 color = vec4(1,0,0,1);

  out vec4 finalColor;

  void main()
  {
    finalColor = color;
  }
);

static void initSimpleShader()
{
  ShaderProgram* shader;
  shader = ShaderProgram::requestShaderProgram("SimpleShader", true);
  shader->addShader(new Shader(SimpleVertexShader, Shader::TYPES::VERTEX_SHADER));
  shader->addShader(new Shader(SimpleFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

  shader->build({ {"vertices", 0}, {"TransMatrix", 3} });
}