#pragma once
#include <OpenGL/Shader.h>

static const std::string SkyboxVertexShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec3 vertexPosition;

	out vec3 Texcoord;

	uniform mat4 projectionMatrix;
	uniform mat4 transformationMatrix;
	uniform mat4 viewMatrix;

	void main()
    {
		mat4 realViewMatrix = viewMatrix * transformationMatrix;
		realViewMatrix[3][0] = 0;
		realViewMatrix[3][1] = 0;
		realViewMatrix[3][2] = 0;
		gl_Position = projectionMatrix * realViewMatrix * vec4(vertexPosition, 1.0f);
		Texcoord = vertexPosition;
	}
)";

static const std::string SkyboxFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec3 Texcoord;
	uniform samplerCube dayTexture;
	uniform samplerCube nightTexture;
	uniform vec3 SkyColor;
	uniform int gradiant;
	uniform vec3 SunDirection;
	
	void main() 
	{
        vec3 envColor = texture(dayTexture, Texcoord).rgb;
        gl_FragColor = vec4(envColor, 1.0f);
        //gl_FragColor = vec4(Texcoord, 1.0f);
	}
)";

static const std::string SkyboxHDRToCubeFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec3 Texcoord;
	uniform sampler2D hdrTexture;
	uniform int gradiant;
	uniform vec3 SunDirection;
	
	const vec2 invAtan = vec2(0.1591, 0.3183);
	vec2 SampleSphericalMap(vec3 v)
	{
		vec2 uv = vec2(atan(v.z, v.x), asin(-v.y));
		uv *= invAtan;
		uv += 0.5;
		return uv;
	}

	void main(void) 
	{
		if(gradiant > 0) 
		{ 
			float sun = clamp(dot(normalize(-SunDirection), normalize(Texcoord)), 0.0, 1.0);
			vec3 col = mix(vec3(0.68,0.68,0.7), vec3(0.52941, 0.80784, 0.98039), Texcoord.y * 0.5 + 0.5);
			col += 0.5*vec3(1.0,0.5,0.1)*pow(sun, 20.0);
			gl_FragColor = vec4(col - 0.3, 1.0);
		} 
		else
		{ 
			vec2 uv = SampleSphericalMap(normalize(Texcoord * vec3(-1, 1, -1))); // make sure to normalize localPos
			vec3 envColor = texture(hdrTexture, uv).rgb;            
			gl_FragColor = vec4(envColor, 1.0f);
		} 

		//gl_FragColor = vec4(1,0,0, 1.0f);
	}
)";

static const std::string SkyboxIrradianceFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec3 Texcoord;
	uniform samplerCube cubeMap;
	const float PI = 3.14159265359;
	const float TWO_PI = PI * 2;
	const float HALF_PI = PI * 0.5;
    const float sampleDelta = 0.025;
    const float totalSamples = (TWO_PI / sampleDelta) * (HALF_PI / sampleDelta); 
    const float invTotalSamples = 1.0 / totalSamples;


	void main(void) 
	{
        vec3 normal = normalize(Texcoord);
    
        vec3 irradiance = vec3(0.0);
        vec3 up    = vec3(0.0, 1.0, 0.0);
        vec3 right = normalize(cross(up, normal));
        up         = cross(normal, right);
    
        for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
        {
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
            {
                // spherical to cartesian (in tangent space)
                vec3 tangentSample = vec3(sin(theta) * cosPhi,  sin(theta) * sinPhi, cos(theta));
                // tangent space to world
                vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

                vec3 envColor = texture(cubeMap, sampleVec).rgb;
                envColor = envColor / (envColor + vec3(1.0));
                envColor = pow(envColor, vec3(1.0/2.2));
                irradiance += envColor * cos(theta) * sin(theta);
            }
        }

        irradiance *= PI * invTotalSamples;
        gl_FragColor = vec4(irradiance, 1.0);
	}
)";


static const std::string SkyboxBRDFVertexShader = Shader::SHADER_VERSION_STR + 
R"(
	layout (location = 0) in vec3 vertexPosition;
	out vec2 TexCoord;
	
	void main()
	{
        TexCoord = (vertexPosition.xy + vec2(1.0)) / 2.0;
        gl_Position = vec4(vertexPosition, 1.0);
	}
)";

static const std::string SkyboxBRDFFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    in vec2 TexCoord;
    
    const float PI = 3.14159265359;
    float RadicalInverse_VdC(uint bits) 
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }
    
    vec2 Hammersley(uint i, uint N)
    {
        return vec2(float(i)/float(N), RadicalInverse_VdC(i));
    }
    
    vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
    {
        float a = roughness*roughness;
        
        float phi = 2.0 * PI * Xi.x;
        float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        
        // from spherical coordinates to cartesian coordinates - halfway vector
        vec3 H;
        H.x = cos(phi) * sinTheta;
        H.y = sin(phi) * sinTheta;
        H.z = cosTheta;
        
        // from tangent-space H vector to world-space sample vector
        vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
        vec3 tangent   = normalize(cross(up, N));
        vec3 bitangent = cross(N, tangent);
        
        vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
        return normalize(sampleVec);
    }
    
    float GeometrySchlickGGX(float NdotV, float roughness)
    {
        // note that we use a different k for IBL
        float a = roughness;
        float k = (a * a) / 2.0;
    
        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;
    
        return nom / denom;
    }
    
    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
        return ggx1 * ggx2;
    }
    
    vec2 IntegrateBRDF(float NdotV, float roughness)
    {
        vec3 V;
        V.x = sqrt(1.0 - NdotV*NdotV);
        V.y = 0.0;
        V.z = NdotV;
    
        float A = 0.0;
        float B = 0.0; 
    
        vec3 N = vec3(0.0, 0.0, 1.0);
        
        const uint SAMPLE_COUNT = 1024u;
        for(uint i = 0u; i < SAMPLE_COUNT; ++i)
        {
            // generates a sample vector that's biased towards the
            // preferred alignment direction (importance sampling).
            vec2 Xi = Hammersley(i, SAMPLE_COUNT);
            vec3 H = ImportanceSampleGGX(Xi, N, roughness);
            vec3 L = normalize(2.0 * dot(V, H) * H - V);
    
            float NdotL = max(L.z, 0.0);
            float NdotH = max(H.z, 0.0);
            float VdotH = max(dot(V, H), 0.0);
    
            if(NdotL > 0.0)
            {
                float G = GeometrySmith(N, V, L, roughness);
                float G_Vis = (G * VdotH) / (NdotH * NdotV);
                float Fc = pow(1.0 - VdotH, 5.0);
    
                A += (1.0 - Fc) * G_Vis;
                B += Fc * G_Vis;
            }
        }
        A /= float(SAMPLE_COUNT);
        B /= float(SAMPLE_COUNT);
        return vec2(A, B);
    }
    

    void main() 
    {
        vec2 integratedBRDF = IntegrateBRDF(TexCoord.x, TexCoord.y);
        gl_FragColor = vec4(integratedBRDF, 0, 1);
    }
)";


static const std::string SkyboxPrefilterFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec3 Texcoord;
	
	uniform samplerCube cubeMap;
	uniform float roughness;
	
	const float PI = 3.14159265359;
    
	float DistributionGGX(vec3 N, vec3 H, float roughness)
	{
		float a = roughness*roughness;
		float a2 = a*a;
		float NdotH = max(dot(N, H), 0.0);
		float NdotH2 = NdotH*NdotH;
	
		float nom   = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = PI * denom * denom;
	
		return nom / denom;
	}
    
	float RadicalInverse_VdC(uint bits) 
	{
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
		bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
		return float(bits) * 2.3283064365386963e-10; // / 0x100000000
	}
    
	vec2 Hammersley(uint i, uint N)
	{
		return vec2(float(i)/float(N), RadicalInverse_VdC(i));
	}
    
	vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
	{
		float a = roughness*roughness;
		
		float phi = 2.0 * PI * Xi.x;
		float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
		float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
		
		// from spherical coordinates to cartesian coordinates - halfway vector
		vec3 H;
		H.x = cos(phi) * sinTheta;
		H.y = sin(phi) * sinTheta;
		H.z = cosTheta;
		
		// from tangent-space H vector to world-space sample vector
		vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
		vec3 tangent   = normalize(cross(up, N));
		vec3 bitangent = cross(N, tangent);
		
		vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
		return normalize(sampleVec);
	}
    
	void main()
	{		
		vec3 N = normalize(Texcoord * vec3(1, 1, -1));
		
		// make the simplyfying assumption that V equals R equals the normal 
		vec3 R = N;
		vec3 V = R;
	
		const uint SAMPLE_COUNT = 1024u;
		vec3 prefilteredColor = vec3(0.0);
		float totalWeight = 0.0;
		
		for(uint i = 0u; i < SAMPLE_COUNT; ++i)
		{
			// generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
			vec2 Xi = Hammersley(i, SAMPLE_COUNT);
			vec3 H = ImportanceSampleGGX(Xi, N, roughness);
			vec3 L  = normalize(2.0 * dot(V, H) * H - V);
	
			float NdotL = max(dot(N, L), 0.0);
			if(NdotL > 0.0)
			{
				// sample from the environment's mip level based on roughness/pdf
				float D   = DistributionGGX(N, H, roughness);
				float NdotH = max(dot(N, H), 0.0);
				float HdotV = max(dot(H, V), 0.0);
				float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 
	
				float resolution = 512.0; // resolution of source cubemap (per face)
				float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
				float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
	
				float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
			
				prefilteredColor += textureLod(cubeMap, L, mipLevel).rgb * NdotL;
				totalWeight      += NdotL;
			}
		}
	
		prefilteredColor = prefilteredColor / totalWeight;
        prefilteredColor = prefilteredColor / (prefilteredColor + vec3(1.0));
        prefilteredColor = pow(prefilteredColor, vec3(1.0/2.2));
        
		gl_FragColor = vec4(prefilteredColor, 1.0);
        //gl_FragColor = vec4(1,0,0,1);
	}
)";