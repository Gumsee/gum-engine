#pragma once
#include <Graphics/Shader.h>

static const std::string ShadowMappingVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 3) in mat4 TransMatrix;

    void main()
    {
        gl_Position = TransMatrix * vec4(vertexPosition, 1.0f);
    }
)";

static const std::string ShadowMappingGeometryShader = Shader::SHADER_VERSION_STR + 
R"(
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
)";

static const std::string ShadowMappingFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    void main()
    {
    }
)";