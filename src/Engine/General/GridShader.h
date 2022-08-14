#pragma once
#include <OpenGL/Shader.h>


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

	const float subdiv = 0.5f;

	float far=gl_DepthRange.far; 
	float near=gl_DepthRange.near;

	float computeDepth(float ndc) 
	{
		return (((far-near) * ndc) + near + far) / 2.0;
	}

	float computeLinearDepth(float ndc)
	{
		float znear = 0.1f;
		float zfar = 100.0f;
		float clip_space_depth = ndc * 2.0 - 1.0; // put back between -1 and 1
		float linearDepth = (2.0 * znear * zfar) / (zfar + znear - clip_space_depth * (zfar - znear)); // get linear value between 0.01 and 100
		return linearDepth / zfar; // normalize
	}

	void main()
	{
    	float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    	vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
		vec4 color = vec4(0);

		if(t > 0)
		{
			vec2 coord = fragPos3D.xz * subdiv; // use the scale variable to set the distance between the lines
			vec2 derivative = fwidth(coord);
			vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
			float line = min(grid.x, grid.y);
			float minimumz = min(derivative.y, 1);
			float minimumx = min(derivative.x, 1);
			color = vec4(0.1, 0.1, 0.1, 1.0 - min(line, 1.0));
			
			if(fragPos3D.x > -subdiv * minimumx && fragPos3D.x < subdiv * minimumx)
				color = vec4(0.0, 0.0, 1.0, 1.0);
				
			if(fragPos3D.z > -subdiv * minimumz && fragPos3D.z < subdiv * minimumz)
				color = vec4(1.0, 0.0, 0.0, 1.0);
		}

		vec4 clip_space_pos = projection * view * vec4(fragPos3D, 1.0);
		float ndc = clip_space_pos.z / clip_space_pos.w;
		gl_FragDepth = computeDepth(ndc);

		float linearDepth = computeLinearDepth(ndc);
    	float fading = max(0, (0.5 - linearDepth));

		color.a *= fading;
		gl_FragColor = color;
	}
)";