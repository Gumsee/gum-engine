#pragma once
#include <Graphics/Shader.h>

static const std::string MotionBlurFragmentShader = GLSL(
  in vec2 Texcoord;
  
  out vec4 FragColor;

  uniform sampler2D texture0;
  uniform vec3 velocity;

  void main(void)
  {
    vec4 color = texture(texture0, Texcoord);
    FragColor = color;
  }
);