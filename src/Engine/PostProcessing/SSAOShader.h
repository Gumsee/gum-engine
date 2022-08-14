#pragma once
#include <OpenGL/Shader.h>

static std::string SSAOFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    varying vec2 vary_Texcoord;
    //out float FragColor;

    uniform sampler2D texPosition;
    uniform sampler2D texNormal;
    uniform sampler2D texNoise;
    uniform mat4 projectionMatrix;

    uniform float power;
    uniform int kernelSize;
    uniform float radius;
    uniform vec3 samples[8];
    uniform vec2 noiseScale;


    const float bias = 0.025f; 
    void main() 
    { 
        vec4 posMap =      texture(texPosition, vary_Texcoord); 
        vec3 fragPos = posMap.xyz;
        float alpha = posMap.a;
        if (alpha <= 0) { gl_FragColor = vec4(0,0,0,0); }
        else
        {
            vec3 normal =       texture(texNormal, vary_Texcoord).xyz; 
            vec3 randomVec =    texture(texNoise, vary_Texcoord * noiseScale).xyz; 
            // create TBN change-of-basis matrix: from tangent-space to view-space 
            vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal)); 
            vec3 bitangent = cross(normal, tangent); 
            mat3 TBN = mat3(tangent, bitangent, normal); 
            // iterate over the sample kernel and calculate occlusion factor 
            float occlusion = 0.0; 
            for(int i = 0; i < kernelSize; ++i) 
            { 
                // get sample position 
                vec3 sample = TBN * samples[i]; // from tangent to view-space 
                sample = fragPos + sample * radius;  
                
                // project sample position (to sample texture) (to get position on screen/texture) 
                vec4 offset = vec4(sample, 1.0); 
                offset = projectionMatrix * offset; // from view to clip-space 
                offset.xyz /= offset.w; // perspective divide 
                offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0 

                // get sample depth 
                vec3 occluderPos = texture(texPosition, offset.xy).xyz;
                float rangeCheck = smoothstep(0.0, 1.0, radius / length(fragPos - occluderPos)); 
                occlusion       += (occluderPos.z >= sample.z + bias ? 1.0 : 0.0) * rangeCheck; 
            } 
            occlusion = 1.0 - (occlusion / kernelSize); 
            occlusion = pow(occlusion, power); 
            gl_FragColor = vec4(occlusion,occlusion,occlusion, alpha);
        }
    }
)";


static std::string SSAOBlurFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    varying vec2 vary_Texcoord;
    //out float FragColor;

    uniform int NoiseSize;
    uniform sampler2D ssaoInput;


    void main() 
    {
        int halfsize = NoiseSize / 2;
        vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
        float result = 0.0;
        for (int i = -halfsize; i < halfsize; ++i) 
        {
            for (int j = -halfsize; j < halfsize; ++j) 
            {
                vec2 offset = vec2(float(i), float(j)) * texelSize;
                result += texture(ssaoInput, vary_Texcoord + offset).r;
            }
        }
        float color = result / float(NoiseSize * NoiseSize);
        gl_FragColor = vec4(color,color,color,1.0f);
    }
)";