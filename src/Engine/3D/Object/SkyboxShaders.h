#pragma once
#include <Graphics/Shader.h>

static const std::string SkyboxVertexShader = GLSL(
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
);

static const std::string HDRFragmentShader = GLSL(
    in vec3 Texcoord;
    out vec4 FragColor;
    uniform samplerCube environmentMap;
    
    void main() 
    {
        vec3 envColor = texture(environmentMap, Texcoord).rgb;
        FragColor = vec4(envColor, 1.0f);
    }
);

#ifdef GUM_PRIMITIVES_MESH_UP_Z
#define TEXTURE_UP Texcoord.z
#define SUN_UP sunDir.z
#define P_UP p.z
#else
#define TEXTURE_UP Texcoord.y
#define SUN_UP sunDir.y
#define P_UP p.y
#endif
static const std::string SkyboxFragmentShader = GLSL(
    in vec3 Texcoord;
    out vec4 color;
    uniform float time = 0.0;
    uniform float cirrus = 0.4;
    uniform float cumulus = 0.8;
    uniform vec3 sunDir;

    const float Br = 0.0025;
    const float Bm = 0.0003;
    const float g  = 0.9800;
    const vec3 nitrogen = vec3(0.650, 0.570, 0.475);
    const vec3 Kr = Br / pow(nitrogen, vec3(4.0));
    const vec3 Km = Bm / pow(nitrogen, vec3(0.84));
    

    float hash(float n)
    {
        return fract(sin(n) * 43758.5453123);
    }

    float noise(vec3 x)
    {
        vec3 f = fract(x);
        float n = dot(floor(x), vec3(1.0, 157.0, 113.0));
        return mix(mix(mix(hash(n +   0.0), hash(n +   1.0), f.x),
                    mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
                mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
                    mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
    }

    const mat3 m = mat3(0.0, 1.60,  1.20, -1.6, 0.72, -0.96, -1.2, -0.96, 1.28);
    float fbm(vec3 p)
    {
        float f = 0.0;
        f += noise(p) / 2; p = m * p * 1.1;
        f += noise(p) / 4; p = m * p * 1.2;
        f += noise(p) / 6; p = m * p * 1.3;
        f += noise(p) / 12; p = m * p * 1.4;
        f += noise(p) / 24;
        return f;
    }

    vec3 getExtinction(vec3 p)
    {
        vec3 day_extinction = exp(-exp(-((P_UP - SUN_UP * 4.0) * (exp(-P_UP * 16.0) + 0.1) / 80.0) / Br) * (exp(-P_UP * 16.0) + 0.1) * Kr / Br) * exp(-P_UP * exp(-P_UP * 8.0 ) * 4.0) * exp(-P_UP * 2.0) * 4.0;
        vec3 night_extinction = vec3(1.0 - exp(-SUN_UP)) * 0.2;
        return mix(day_extinction, night_extinction, SUN_UP * 0.2 + 0.5);
    }

    void main()
    {
        color.a = 1.0;
        if(TEXTURE_UP < -0.1)
        {
            vec3 p = Texcoord;
            P_UP = -0.1;
            vec3 extinction = getExtinction(p);
            color.rgb = extinction * (1.0 - smoothstep(0.0, 1.0, -TEXTURE_UP * 0.5));
            return;
        }
        
        // Atmosphere Scattering
        vec3 extinction = getExtinction(Texcoord);
        float mu = dot(normalize(Texcoord), normalize(-sunDir));
        float rayleigh = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu);
        vec3 mie = (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm);
        color.rgb = rayleigh * mie * extinction;

        // Cirrus Clouds
        float density = smoothstep(1.0 - cirrus, 1.0, fbm(Texcoord.xyz / TEXTURE_UP * 2.0 + time * 0.05)) * 0.3;
        color.rgb = mix(color.rgb, extinction * 4.0, density * max(TEXTURE_UP, 0.0));

        // Cumulus Clouds
        for (int i = 0; i < 3; i++)
        {
            float density = smoothstep(1.0 - cumulus, 1.0, fbm((0.7 + float(i) * 0.01) * Texcoord.xyz / TEXTURE_UP + time * 0.3));
            color.rgb = mix(color.rgb, extinction * density * 5.0, min(density, 1.0) * max(TEXTURE_UP, 0.0));
        }
    }
);

static const std::string SkyboxHDRToCubeFragmentShader = GLSL(
    in vec3 Texcoord;
    out vec4 FragColor;
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
            float sun = clamp(dot(normalize(SunDirection * vec3(1,-1,1)), normalize(Texcoord)), 0.0, 1.0);
            vec3 col = mix(vec3(0.68,0.68,0.7), vec3(0.52941, 0.80784, 0.98039), -TEXTURE_UP * 0.5 + 0.5);
            col += 0.5*vec3(1.0,0.5,0.1)*pow(sun, 20.0);
            FragColor = vec4(col - 0.3, 1.0);
        } 
        else
        { 
            vec2 uv = SampleSphericalMap(normalize(Texcoord * vec3(-1, 1, -1))); // make sure to normalize localPos
            vec3 envColor = texture(hdrTexture, uv).rgb;            
            FragColor = vec4(envColor, 1.0f);
        } 

        //FragColor = vec4(1,0,0, 1.0f);
    }
);

#ifdef GUM_PRIMITIVES_MESH_UP_Z
    #define UP vec3(0.0, 0.0, 1.0)
    #define TEXCOORD vec3(Texcoord.x, Texcoord.y, -Texcoord.z)
#else
    #define UP vec3(0.0, 1.0, 0.0)
    #define TEXCOORD Texcoord
#endif
static const std::string SkyboxIrradianceFragmentShader = GLSL(
  out vec4 FragColor;
	in vec3 Texcoord;

  uniform samplerCube cubeMap;

  // Mirror binary digits about the decimal point
float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// Randomish sequence that has pretty evenly spaced points
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
vec2 hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}  

vec3 importanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up = abs(N.z) < 0.999 ? vec3 (0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

void main()
{
    // Not the normal of the cube, but the normal that we're calculating the irradiance for
    vec3 normal = normalize(TEXCOORD);
    vec3 N = normal;

    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    // ------------------------------------------------------------------------------

    const uint SAMPLE_COUNT = 16384u;
    float totalWeight = 0.0;
    for (uint i = 0u; i < SAMPLE_COUNT; ++i) {
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H = importanceSampleGGX(Xi, N, 1.0);

        // NdotH is equal to cos(theta)
        float NdotH = max(dot(N, H), 0.0);
        // With roughness == 1 in the distribution function we get 1/pi
        float D = 1.0 / PI;
        float pdf = (D * NdotH / (4.0)) + 0.0001; 

        float resolution = 1024.0; // resolution of source cubemap (per face)
        // with a higher resolution, we should sample coarser mipmap levels
        float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
        // as we take more samples, we can sample from a finer mipmap.
        // And places where H is more likely to be sampled (higher pdf) we
        // can use a finer mipmap, otherwise use courser mipmap.
        // The tutorial treats this portion as optional to reduce noise but I think it's
        // actually necessary for importance sampling to get the correct result
        float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

        float mipLevel = 0.5 * log2(saSample / saTexel); 

        vec3 envColor = textureLod(cubeMap, H, mipLevel).rgb.rgb;
        envColor = envColor / (envColor + vec3(1.0));
        envColor = pow(envColor, vec3(1.0/2.2));

        irradiance += envColor * NdotH;
        // irradiance += texture(environmentMap, H).rgb * NdotH;
        totalWeight += NdotH;
    }
    irradiance = (PI * irradiance) / totalWeight;
    FragColor = vec4(irradiance, 1.0);
  }

  /*void main()
  {		
    // The world vector acts as the normal of a tangent surface
      // from the origin, aligned to WorldPos. Given this normal, calculate all
      // incoming radiance of the environment. The result of this radiance
      // is the radiance of light coming from -Normal direction, which is what
      // we use in the PBR shader to sample irradiance.
      vec3 N = normalize(TEXCOORD);

      vec3 irradiance = vec3(0.0);   
      
      // tangent space calculation from origin point
      vec3 up    = UP;
      vec3 right = normalize(cross(up, N));
      up         = normalize(cross(N, right));
        
      float sampleDelta = 0.025;
      float nrSamples = 0.0;
      for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
      {
          for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
          {
              // spherical to cartesian (in tangent space)
              vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
              // tangent space to world
              vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

              irradiance += texture(cubeMap, sampleVec).rgb * cos(theta) * sin(theta);
              nrSamples++;
          }
      }
      irradiance = PI * irradiance * (1.0 / float(nrSamples));
      
      FragColor = vec4(irradiance, 1.0);
  }*/
);
/*GLSL(
	in vec3 Texcoord;
    out vec4 FragColor;
	uniform samplerCube cubeMap;
	//const float PI = 3.14159265359;
	const float TWO_PI = PI * 2;
	const float HALF_PI = PI * 0.5;
    const float sampleDelta = 0.025;
    const float totalSamples = (TWO_PI / sampleDelta) * (HALF_PI / sampleDelta); 
    const float invTotalSamples = 1.0 / totalSamples;


	void main(void) 
	{
        vec3 normal = normalize(Texcoord);
    
        vec3 irradiance = vec3(0.0);
        vec3 up    = UP;
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
        FragColor = vec4(irradiance, 1.0);
	}
);*/


static const std::string SkyboxBRDFVertexShader = GLSL(
	layout (location = 0) in vec3 vertexPosition;
	out vec2 TexCoord;
	
	void main()
	{
        TexCoord = (vertexPosition.xy + vec2(1.0)) / 2.0;
        gl_Position = vec4(vertexPosition, 1.0);
	}
);

static const std::string SkyboxBRDFFragmentShader = GLSL(
    in vec2 TexCoord;
    out vec4 FragColor;
    
    //const float PI = 3.14159265359;
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
        FragColor = vec4(integratedBRDF, 0, 1);
    }
);


static const std::string SkyboxPrefilterFragmentShader = GLSL(
	in vec3 Texcoord;
    out vec4 FragColor;
	
	uniform samplerCube cubeMap;
	uniform float roughness;
	
	//const float PI = 3.14159265359;
    
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
        
		FragColor = vec4(prefilteredColor, 1.0);
        //FragColor = vec4(1,0,0,1);
        //FragColor = texture(cubeMap, Texcoord);
	}
);