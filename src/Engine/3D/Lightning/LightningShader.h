#pragma once
#include <Graphics/Shader.h>

static const std::string LightningFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
	in vec2 Texcoord;

    out vec4 FragColor;
    
	struct Light {
	    vec3 Position;
	    vec3 Color;
	};

	uniform samplerCube ReflectionMap;
	uniform samplerCube IrradianceMap;
	uniform samplerCube prefilterMap;
	uniform sampler2D   brdfLUT;  
	uniform sampler2D gPosition;
	uniform sampler2D gNormal;
	uniform sampler2D gAlbedo;
	uniform sampler2D ssao;
	uniform sampler2D gObjectData;
	uniform sampler2D ShadowMap;

	uniform vec3 SunColor;
	uniform vec3 SunDirection;
	uniform mat4 ToShadowMap;
	uniform int ShadowMapSize;
	uniform mat4 viewmat;
	uniform vec3 viewPos;
	uniform vec2 pixelSize;
	uniform Light lights[128];
	uniform int numLights;

	const float PI = 3.14159265359;
	float shadowDistance = 150;
	const float transitionDistance = 10;



	//=====================================================================================
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
	//=====================================================================================
	float GeometrySchlickGGX(float NdotV, float roughness)
	{
	    float r = (roughness + 1.0);
	    float k = (r*r) / 8.0;
	
	    float nom   = NdotV;
	    float denom = NdotV * (1.0 - k) + k;
	
	    return nom / denom;
	}
	//=====================================================================================
	float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
	{
	    float NdotV = max(dot(N, V), 0.0);
	    float NdotL = max(dot(N, L), 0.0);
	    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
	    return ggx1 * ggx2;
	}
	//=====================================================================================
	vec3 fresnelSchlick(float cosTheta, vec3 F0)
	{
	    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
	}
	//=====================================================================================
	vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
	{
	    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
	} 
	
	//
	//ShadowMapping
	//
	float sampleShadowMap(sampler2D shadowmap, vec2 shadowcoords, float compare)
	{
	    return step(compare, texture(shadowmap, shadowcoords.xy).r);
	}

	float SampleShadowMapLinear(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
	{
	       vec2 pixelPos = coords/texelSize + vec2(0.5);
	       vec2 fracPart = fract(pixelPos);
	       vec2 startTexel = (pixelPos - fracPart) * texelSize;

	       float blTexel = sampleShadowMap(shadowMap, startTexel, compare);
	       float brTexel = sampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, 0.0), compare);
	       float tlTexel = sampleShadowMap(shadowMap, startTexel + vec2(0.0, texelSize.y), compare);
	       float trTexel = sampleShadowMap(shadowMap, startTexel + texelSize, compare);

	       float mixA = mix(blTexel, tlTexel, fracPart.y);
	       float mixB = mix(brTexel, trTexel, fracPart.y);

	       return mix(mixA, mixB, fracPart.x);
	}

	float SampleShadowMapPCF(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
	{
	       const float NUM_SAMPLES = 3.0f;
	       const float SAMPLES_START = (NUM_SAMPLES-1.0f)/2.0f;
	       const float NUM_SAMPLES_SQUARED = NUM_SAMPLES*NUM_SAMPLES;

	       float result = 0.0f;
	       for(float y = -SAMPLES_START; y <= SAMPLES_START; y += 1.0f)
	       {
				for(float x = -SAMPLES_START; x <= SAMPLES_START; x += 1.0f)
				{
					vec2 coordsOffset = vec2(x,y) * texelSize;
					result += SampleShadowMapLinear(shadowMap, coords + coordsOffset, compare, texelSize);
				}
	       }
	       return result/NUM_SAMPLES_SQUARED;
	}

	float calcShadowAmount(sampler2D shadowmap, vec4 startShadowMapCoords)
	{
	    vec3 shadowmapcoords = startShadowMapCoords.xyz / startShadowMapCoords.w;
	    shadowmapcoords = shadowmapcoords * 0.5 + 0.5;

	    if(shadowmapcoords.z > 1.0)
	    {
	        return 1.0;
	    }
	    else
	    {
	        return SampleShadowMapPCF(shadowmap, shadowmapcoords.xy, shadowmapcoords.z - 5.0/ShadowMapSize, vec2(1.0/ShadowMapSize)) / 2;
	    }
	}

	const float MAX_REFLECTION_LOD = 4.0;

	void main()
	{
	    // retrieve data from gbuffer
	       vec4 additionalData = texture(gObjectData, Texcoord);
	       vec4 PositionViewSpace = texture(gPosition, Texcoord);
	       vec3 Position = (inverse(viewmat) * PositionViewSpace).xyz; //WorldSpace
	       vec3 NormalViewSpace =   texture(gNormal, Texcoord).xyz;
	       vec3 Normal =   (inverse(viewmat) * vec4(NormalViewSpace, 0.0f)).xyz; //WorldSpace
	       vec4 Albedo =   texture(gAlbedo, Texcoord);

	    //AO
	    float AmbientOcclusion = texture(ssao, Texcoord).r;
	    AmbientOcclusion += 0.01;
	    float AmbientOcclusionFactor = additionalData.a;// * AmbientOcclusion;

	    float roughnessFactor = additionalData.r;
	    float reflectionFactor = additionalData.b;
	    //float refractionFactor = additionalData.b;
	    float specularStrength = additionalData.g;
	    float metallic = specularStrength;
	    vec3 viewDir = normalize(viewPos - Position);

        //
	    //ShadowMapping
        //
	    vec4 shadowspace = ToShadowMap * vec4(Position, 1.0f);
	    vec3 shadowmapcoords = shadowspace.xyz / shadowspace.w;
	    float shadow = calcShadowAmount(ShadowMap, shadowspace);

	    vec3 F0 = vec3(0.04); 
	    F0 = mix(F0, Albedo.rgb, metallic);

        //
	    //Point Lights
        //
	    vec3 light = vec3(0.0);
	    for(int i = 0; i < numLights; i++)
	    {
	        // calculate per-light radiance
	        vec3 LightDir = normalize(lights[i].Position - Position);
	        vec3 Half = normalize(viewDir + LightDir);
	        float distance = length(lights[i].Position - Position);
	        float attenuation = 1.0 / (distance * distance);
	        vec3 radiance = lights[i].Color * attenuation;

	        // Cook-Torrance BRDF
	        float NDF = DistributionGGX(Normal, Half, roughnessFactor);   
	        float G   = GeometrySmith(Normal, viewDir, LightDir, roughnessFactor);      
	        vec3 F    = fresnelSchlick(max(dot(Half, viewDir), 0.0), F0);

	        vec3 nominator    = NDF * G * F;// * shadow; 
	        float denominator = 4 * max(dot(Normal, viewDir), 0.0) * max(dot(Normal, LightDir), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	        vec3 specular = nominator / denominator;

	        vec3 kS = F;
	        vec3 kD = vec3(1.0) - kS;
	        kD *= 1.0 - metallic;

	        // scale light by NdotL
	        float NdotL = max(dot(Normal, LightDir), 0.0);

	        // add to outgoing radiance Lo
	        light += (kD * Albedo.rgb / PI + specular) * radiance * NdotL;
	    }

        //
        //Sunlight
        //
	    vec3 LightDir = normalize(-SunDirection);
	    vec3 Half = normalize(viewDir + LightDir);
	    vec3 radiance = SunColor;

	    // Cook-Torrance BRDF
	    float NDF = DistributionGGX(Normal, Half, roughnessFactor);   
	    float G   = GeometrySmith(Normal, viewDir, LightDir, roughnessFactor);    
	    vec3 F    = fresnelSchlick(max(dot(Half, viewDir), 0.0), F0);

	    vec3 nominator    = NDF * G * F; 
	    float denominator = 4 * max(dot(Normal, viewDir), 0.0) * max(dot(Normal, LightDir), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	    vec3 specular = nominator / denominator;

	    // kS is equal to Fresnel
	    vec3 kS = F;
	    vec3 kD = vec3(1.0) - kS;
	    kD *= 1.0 - metallic;        
	    // scale light by NdotL
	    float NdotL = max(dot(Normal, LightDir), 0.0);        

	    // add to outgoing radiance Lo
	    vec3 sunlight = (kD * Albedo.rgb / PI + specular/* * shadow*/) * radiance * NdotL;


        //
        //Ambient Light
        //
	    vec3 AmbientkS = fresnelSchlickRoughness(max(dot(Normal, viewDir), 0.0), F0, roughnessFactor);// * shadow;
	    vec3 AmbientkD = 1.0 - AmbientkS;  
	    vec3 irradiance = texture(IrradianceMap, Normal).rgb;
	    vec3 diffuse      = irradiance * Albedo.rgb;

	    vec3 prefilteredColor = textureLod(prefilterMap, reflect(-viewDir, Normal),  roughnessFactor * MAX_REFLECTION_LOD).rgb;    
	    vec2 brdf  = texture(brdfLUT, vec2(max(dot(Normal, viewDir), 0.0), roughnessFactor)).rg;
	    vec3 AmbientSpecular = prefilteredColor * (AmbientkS * brdf.x + brdf.y);
	    vec3 ambient = (AmbientkD * diffuse + AmbientSpecular) * AmbientOcclusionFactor;



	    vec3 color = (ambient + sunlight/* * shadow*/ + light);
        //color = light + ambient * shadow;
        //color = irradiance;
        
	    color = mix(color, texture(ReflectionMap, reflect(viewDir, normalize(Normal))).rgb, reflectionFactor);

        //In HDR shader
	    //color = color / (color + vec3(1.0)); // HDR tonemapping
	    //color = pow(color, vec3(1.0/2.2)); // gamma correct
	    //color = vec3(1,0,0);
	

	    float alpha = 1.0f;
	    if(Normal == vec3(0.0f)) { alpha = 0.0f; }

	    FragColor = vec4(color, alpha);
	    //FragColor = mix(FragColor, texture(ReflectionMap, reflect(viewDir, normalize(Normal))), reflectionFactor);
	    //FragColor = texture(ShadowMap, shadowmapcoords.xy);
    	//FragColor = vec4(color, 1.0f);
    	//FragColor = vec4(light, alpha);
	}
)";