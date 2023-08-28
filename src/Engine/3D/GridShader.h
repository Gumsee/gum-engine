#pragma once
#include <Graphics/Shader.h>


static const std::string GridVertexShader = Shader::SHADER_VERSION_STR + 
R"(
	uniform mat4 viewMatrix;
	uniform mat4 projectionMatrix;

	out vec3 nearPoint;
	out vec3 farPoint;
	out mat4 projection;
	out mat4 view;

	vec2 gridPlane[6] = vec2[](
		vec2(-1, -1), vec2( 1,  1), vec2( 1, -1),
		vec2( 1,  1), vec2(-1, -1), vec2(-1,  1)
	);

	vec3 unprojectPoint(float x, float y, float z, mat4 view, mat4 projection) 
	{
		mat4 viewInv = inverse(view);
		mat4 projInv = inverse(projection);
		vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
		return unprojectedPoint.xyz / unprojectedPoint.w;
	}

	void main()
	{
		vec2 p = gridPlane[gl_VertexID].xy;
		nearPoint = unprojectPoint(p.x, p.y, 0.0, viewMatrix, projectionMatrix).xyz; // unprojecting on the near plane
		farPoint  = unprojectPoint(p.x, p.y, 1.0, viewMatrix, projectionMatrix).xyz; // unprojecting on the far plane
		projection = projectionMatrix;
		view = viewMatrix;
		gl_Position = vec4(p, 0.0, 1.0); // using directly the clipped coordinates

		//gl_Position = projectionMatrix * viewMatrix * vec4(gridPlane[gl_VertexID].xy, 0, 1.0);
	}
)";

static const std::string GridFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec3 nearPoint;
	in vec3 farPoint;
	in mat4 projection;
	in mat4 view;

    out vec4 FragColor;

	const float indicatorThickness = 2.0f;

	float far= 5000; 
	float near= 0.1;

    vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
        vec2 coord = fragPos3D.xz * scale;
        vec2 derivative = fwidth(coord);
        vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
        float line = min(grid.x, grid.y);
        float minimumz = min(derivative.y, 1);
        float minimumx = min(derivative.x, 1);
        vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
        // z axis
        if(fragPos3D.x > -indicatorThickness * minimumx && fragPos3D.x < indicatorThickness * minimumx)
            color.z = 1.0;
        // x axis
        if(fragPos3D.z > -indicatorThickness * minimumz && fragPos3D.z < indicatorThickness * minimumz)
            color.x = 1.0;
        return color;
    }
    float computeDepth(vec3 pos) {
        vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
        return (clip_space_pos.z / clip_space_pos.w);
    }

    float linearize_depth(float d,float zNear,float zFar)
    {
        return zNear * zFar / (zFar + d * (zNear - zFar));
    }

    void main() {
        float t = -nearPoint.y / (farPoint.y - nearPoint.y);
        vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

        gl_FragDepth = computeDepth(fragPos3D);

        float linearDepth = linearize_depth(gl_FragDepth, near, far);
        float fading = 1.0 - t * 200.0;

        /*if(linearDepth < 0.2)
            FragColor = grid(fragPos3D, 100, true);
        else if(linearDepth < 0.4)
            FragColor = grid(fragPos3D, 10, true);
        else if(linearDepth < 1)
            FragColor = grid(fragPos3D, 1, true);
        else
            FragColor = grid(fragPos3D, 0.1, true);*/

        FragColor = grid(fragPos3D, 1, true);
        FragColor *= float(t > 0);
        FragColor.a *= clamp(0.0f, 1.0f, fading);
    }
)";