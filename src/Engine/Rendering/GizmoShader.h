#pragma once
#include <Graphics/Shader.h>

static const std::string GizmoVertexShader = GLSL(
    layout (location = 0) in vec3 vertices;
    layout (location = 1) in vec3 colors;
    layout (location = 2) in mat4 TransMatrix;

    out vec3 fsColor;

    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;

    void main()
    {
        fsColor = colors;
        gl_Position = projectionMatrix * viewMatrix * TransMatrix * vec4(vertices, 1.0);
    }
);

static const std::string GizmoFragmentShader = GLSL(
    in vec3 fsColor;

    out vec4 finalColor;

    void main()
    {
        finalColor = vec4(fsColor, 1.0);
    }
);