#pragma once
#include <Graphics/Shader.h>

static const std::string ShadowMappingVertexShader = GLSL(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 3) in mat4 TransMatrix;

    uniform mat4 lightSpaceMatrix;
    uniform int isCascaded;

    void main()
    {
        if(isCascaded > 0)
            gl_Position = TransMatrix * vec4(vertexPosition, 1.0f);
        else
            gl_Position = lightSpaceMatrix * TransMatrix * vec4(vertexPosition, 1.0f);
    }
);

static const std::string ShadowMappingGeometryShader = GLSL(

    layout(triangles, invocations = 5) in;
    layout(triangle_strip, max_vertices = 3) out;
    
    uniform mat4 lightSpaceMatrices[5];
    

    void main()
    {          
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
            gl_Layer = gl_InvocationID;
            EmitVertex();
        }
        EndPrimitive();
    } 
);

static const std::string ShadowMappingFragmentShader = GLSL(
    void main()
    {
    }
);