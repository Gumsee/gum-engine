#pragma once
#include <Graphics/Shader.h>

static const std::string LightningFragmentShader = GLSL(
	in vec2 Texcoord;

    out vec4 FragColor;

	uniform samplerCube ReflectionMap;
	uniform samplerCube IrradianceMap;
	uniform samplerCube prefilterMap;
	uniform sampler2D   brdfLUT;  
	uniform sampler2D gPosition;
	uniform sampler2D gNormal;
	uniform sampler2D gAlbedo;
	uniform sampler2D ssao;
	uniform sampler2D gObjectData;
	uniform sampler2DArray ShadowMap;

	uniform vec3 SunColor;
	uniform vec3 SunDirection;
	#ifndef GUM_ENGINE_NO_SHADOWMAP
    uniform int cascadeCount;
    uniform float cascadePlaneDistances[5];
	uniform mat4 shadowMapMatrices[5];
	#endif
	uniform int farPlane;
	uniform mat4 viewmat;
	uniform vec3 viewPos;
	uniform vec2 pixelSize;
    uniform vec3 lightPosition[128];
    uniform vec3 lightColor[128];
	uniform int numLights;

	//const float PI = 3.14159265359;
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
	#ifndef GUM_ENGINE_NO_SHADOWMAP
    float ShadowCalculation(vec3 fragPosWorldSpace, vec3 normal)
    {
        // select cascade layer
        vec4 fragPosViewSpace = viewmat * vec4(fragPosWorldSpace, 1.0);
        float depthValue = abs(fragPosViewSpace.z);

        int layer = -1;
        for (int i = 0; i < cascadeCount; ++i)
        {
            if (depthValue < cascadePlaneDistances[i])
            {
                layer = i;
                break;
            }
        }
        if (layer == -1)
        {
            layer = cascadeCount;
        }

        vec4 fragPosLightSpace = shadowMapMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
        // perform perspective divide
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        // transform to [0,1] range
        projCoords = projCoords * 0.5 + 0.5;

        // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;

        // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
        if (currentDepth > 1.0)
        {
            return 0.0;
        }
        // calculate bias (based on depth map resolution and slope)
        float bias = max(0.05 * (1.0 - dot(normal, -SunDirection)), 0.005);
        const float biasModifier = 0.5f;
        if (layer == cascadeCount)
        {
            bias *= 1 / (farPlane * biasModifier);
        }
        else
        {
            bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
        }

        // PCF
        float shadow = 0.0;
        vec2 texelSize = 1.0 / vec2(textureSize(ShadowMap, 0));
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(ShadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
                shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        //shadow /= 9.0;
            
        return shadow;
    }
	#endif

	const float MAX_REFLECTION_LOD = 4.0;

    float linearize_depth(float depth)
    {
        float near_plane = 0.1;
        float far_plane = 5000;
        float z = depth * 2.0 - 1.0; // Back to NDC 
        return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
    }

	void main()
	{
	    // retrieve data from gbuffer
        vec3 Position =       texture(gPosition, Texcoord).xyz; //WorldSpace
        vec3 Normal =         texture(gNormal, Texcoord).xyz; //WorldSpace
        vec4 additionalData = texture(gObjectData, Texcoord);
        vec4 Albedo =         texture(gAlbedo, Texcoord);

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
		#ifndef GUM_ENGINE_NO_SHADOWMAP
	    float shadow = 1.0 - ShadowCalculation(Position, Normal);
		#endif

	    vec3 F0 = vec3(0.04); 
	    F0 = mix(F0, Albedo.rgb, metallic);

        //
	    //Point Lights
        //
	    vec3 light = vec3(0.0);
	    for(int i = 0; i < numLights; i++)
	    {
	        // calculate per-light radiance
	        vec3 LightDir = normalize(lightPosition[i] - Position);
	        vec3 Half = normalize(viewDir + LightDir);
	        float distance = length(lightPosition[i] - Position);
	        float attenuation = 1.0 / (distance * distance);
	        vec3 radiance = lightColor[i] * attenuation;

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
            //light += radiance;
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


        //ambient = vec3(0); // needs fix
		#ifndef GUM_ENGINE_NO_SHADOWMAP
	    vec3 color = (ambient + sunlight * shadow + light);
		#else
		vec3 color = (ambient + sunlight + light);
		#endif
        
	    color = mix(color, texture(ReflectionMap, -reflect(viewDir, normalize(Normal))).rgb, reflectionFactor);

	
      //color = vec3(ambient);

	    float alpha = 1.0f;
	    if(Normal == vec3(0.0f)) { alpha = 0.0; }

	    FragColor = vec4(color, alpha);
	}
);