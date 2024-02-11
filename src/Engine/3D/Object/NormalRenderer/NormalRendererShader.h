#pragma once
#include <Graphics/Shader.h>

static const std::string NormalRendererVertexShader = GLSL(
    layout (location = 0) in vec3 vertices;
    layout (location = 2) in vec3 normals;
    layout (location = 3) in mat4 transMatrices;

    out VS_OUT {
        vec3 normal;
    } vs_out;

	uniform mat4 viewMatrix;

	void main()
    {
        mat4 viewModel = viewMatrix * transMatrices;
	    gl_Position = viewModel * vec4(vertices, 1.0);

        mat3 normalMatrix = mat3(transpose(inverse(viewModel)));
        vs_out.normal = normalize(vec3(vec4(normalMatrix * normals, 0.0)));
	}
);

static const std::string NormalRendererGeometryShader = GLSL(
    layout (triangles) in;
    layout (line_strip, max_vertices = 6) out;

    in VS_OUT {
        vec3 normal;
    } gs_in[];
    
    uniform mat4 projectionMatrix;
    uniform float length = 0.4;

    void GenerateLine(int index)
    {
        gl_Position = projectionMatrix * gl_in[index].gl_Position;
        EmitVertex();
        gl_Position = projectionMatrix * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * length);
        EmitVertex();
        EndPrimitive();
    }

    void main()
    {
        GenerateLine(0); // first vertex normal
        GenerateLine(1); // second vertex normal
        GenerateLine(2); // third vertex normal
    }  
);

static const std::string NormalRendererFragmentShader = GLSL(
	in vec3 Texcoord;
    out vec4 FragColor;
	
	void main() 
	{
        FragColor = vec4(1,1,0,1);
	}
);