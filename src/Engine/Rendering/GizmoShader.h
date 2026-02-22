#pragma once
#include <Graphics/Shader.h>

static const std::string GizmoVertexShader = GLSL(
    layout (location = 0) in vec2 vertices;
    layout (location = 1) in vec4 colors;

    out vec4 color;

    void main()
    {
        color = colors;
        gl_Position = vec4(vertices, 0, 1);
    }
);

static const std::string GizmoFragmentShader = GLSL(
    out vec4 finalColor;
    in vec4 color;

    void main()
    {
        finalColor = vec4(1,0,0,1);
    }
);