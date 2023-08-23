#pragma once
#include "../Shaders/ShaderManager.h"


static const std::string ThicklinesVertexShader = Shader::SHADER_VERSION_STR + 
R"(
layout (location = 0) in vec3 points;
layout (location = 3) in mat4 TransMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out mat4 projMatrix;
out mat4 viewMat;

void main()
{
    projMatrix = projectionMatrix;
    viewMat = viewMatrix;
	gl_Position = projectionMatrix * viewMatrix * TransMatrix * vec4(points, 1.0);
}
)";

static const std::string ThicklinesGeometryShader = Shader::SHADER_VERSION_STR + 
R"(
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in mat4 projMatrix[];
in mat4 viewMat[];
    
uniform vec2  viewportSize;
uniform float u_thickness = 30;

void main()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec2 dir    = normalize((p2.xy/p2.w - p1.xy/p1.w) * viewportSize);
    vec2 offset = vec2(-dir.y, dir.x) * u_thickness / viewportSize;

    gl_Position = p1 + vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 + vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 - vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
}
)";

static const std::string ThicklinesFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
uniform vec4 color = vec4(1,0,0,1);

out vec4 finalColor;

void main()
{
	finalColor = color;
}
)";

static void initThicklinesShader()
{
    ShaderProgram* shader;
    shader = new ShaderProgram();
    shader->addShader(new Shader(ThicklinesVertexShader, Shader::TYPES::VERTEX_SHADER));
    shader->addShader(new Shader(ThicklinesGeometryShader, Shader::TYPES::GEOMETRY_SHADER));
    shader->addShader(new Shader(ThicklinesFragmentShader, Shader::TYPES::FRAGMENT_SHADER));

    shader->build("ThicklinesShader", { {"vertices", 0}, {"TransMatrix", 3} });
    shader->addUniform("color");
    shader->addUniform("viewportSize");

    Gum::ShaderManager::addShaderProgram(shader);
}



/*static const std::string CurveVertexShader = Shader::SHADER_VERSION_STR + 
R"(
layout (location = 0) in vec3 points;

void main()
{
	gl_Position = vec4(points, 1.0);
}
)";

static const std::string CurveFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
uniform vec4 tint;

out vec4 finalColor;

void main()
{
	//finalColor = tint;
    finalColor = vec4(1,0,0,1);
}
)";

static const std::string CurveTessellationControlShader = Shader::SHADER_VERSION_STR + 
R"(
layout(vertices = 4) out;

uniform int segmentCount;
uniform int stripCount;

void main()
{

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	gl_TessLevelOuter[1] = segmentCount;
	gl_TessLevelOuter[0] = 1.5f;
}
)";

static const std::string CurveTessellationShader = Shader::SHADER_VERSION_STR + 
R"(
layout(isolines) in;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{

	float t = gl_TessCoord.x;

	//points
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;
	vec3 p3 = gl_in[3].gl_Position.xyz;

	//coefficients
	float b0 = pow((1.0 - t),3);
	float b1 = 3.0 * t * (1.0 - t) * (1.0 - t);
	float b2 = 3.0 * t * t * (1.0 - t);
	float b3 = pow(t, 3);

	//curve evaluation
	vec3 p = b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;

	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(p, 1.0);
    //gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(b0 * p0 + b1 * p1, 1.0);

}
)";*/