#pragma once
#include <Graphics/Shader.h>

static const std::string GBufferVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 1) in vec2 TextureCoords;
    layout (location = 2) in vec3 Normals;
    layout (location = 3) in mat4 TransMatrix;
    layout (location = 7) in vec3 tangentNormals;
    layout (location = 8) in ivec3 jointIndices;
    layout (location = 9) in vec3 weights;

    out VS_OUT
    {
        vec2 Texcoord;
        vec3 worldNormal;
        vec4 FragPos;
        mat3 TBN;
        vec3 viewPos;
        vec3 weights;
        flat ivec3 jointIndices;
        flat int isSkel;
    } vs_out;

    //Skeletal Stuff
    const int MAX_JOINTS = 50;//max joints allowed in a skeleton
    const int MAX_WEIGHTS = 3;//max number of joints that can affect a vertex
    const int MAX_BONES = 100;
    uniform mat4 gBones[MAX_BONES];
    uniform int isSkeletal;
    uniform mat4 transformationMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;
    uniform int TextureMultiplier;
    uniform vec4 ClipPlane;
    uniform vec3 viewPos;
    uniform int flipNormal;
    uniform int isInstanced;

    void main()
    {
        vs_out.Texcoord = TextureCoords * TextureMultiplier;
        vec3 FinalVertexPosition = vertexPosition;
        vec4 totalLocalPos = vec4(0.0);

        //Is Skeletal Check
        vs_out.isSkel = isSkeletal;
        if(isSkeletal == 1)
        {
            vs_out.weights = weights;
            vs_out.jointIndices = jointIndices;
            for(int i = 0; i < MAX_WEIGHTS; i++)
            {
                mat4 jointTransform = gBones[jointIndices[i]];
                vec4 posePosition = jointTransform * vec4(vertexPosition, 1.0);
                totalLocalPos += posePosition * weights[i];
            }
            FinalVertexPosition = totalLocalPos.xyz;
        }
        vec4 worldPosition = TransMatrix * vec4(FinalVertexPosition, 1.0f);
        vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;
        gl_ClipDistance[0] = dot(worldPosition, ClipPlane);

        vec3 T = normalize(vec3(TransMatrix * vec4(tangentNormals, 0.0)));
        vec3 N = normalize(vec3(TransMatrix * vec4(Normals,        0.0)));
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(N, T);
        vs_out.TBN = mat3(T, B, N);

        vs_out.worldNormal = (TransMatrix * vec4(Normals, 0.0f)).xyz;
        vs_out.FragPos = worldPosition;
        vs_out.viewPos = viewPos;
        gl_Position = positionRelativeToCam;
    }
)";


static const std::string GBufferGeometryShader = Shader::SHADER_VERSION_STR + 
R"(
    
)";


static const std::string GBufferFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) out vec4 gPosition;
    layout (location = 1) out vec4 gNormal;
    layout (location = 2) out vec4 gAlbedoSpec;
    layout (location = 3) out vec4 gObjectData;

    in VS_OUT
    {
        vec2 Texcoord;
        vec3 worldNormal;
        vec4 FragPos;
        mat3 TBN;
        vec3 viewPos;
        vec3 weights;
        flat ivec3 jointIndices;
        flat int isSkel;
    } fs_in;

    uniform sampler2D texture0;
    uniform samplerCube Enviorment;
    uniform sampler2D normalmap;
    uniform sampler2D specularmap;
    uniform sampler2D reflectionmap;
    uniform sampler2D refractionmap;
    uniform sampler2D roughnessmap;
    uniform sampler2D ambientOcclusionmap;
    uniform sampler2D Displacement;

    uniform vec4 color;

    uniform int hasTexture;
    uniform int hasNormalMap;
    uniform int hasSpecularMap;
    uniform int hasReflectionMap;
    uniform int hasRefractionMap;
    uniform int hasRoughnessMap;
    uniform int hasAmbientOcclusionMap;
    uniform int hasDisplacementMap;

    uniform float specularity;
    uniform float reflectivity;
    uniform float refractivity;
    uniform float roughness;

    const float near = 0.1;
    const float far = 5000.0; 
    const float heightScale = 0.1;

    vec2 ParallaxMapping(vec2 texCoords, vec3 viewdir)
    {
        // number of depth layers
        const float minLayers = 8;
        const float maxLayers = 32;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewdir)));  
        // calculate the size of each layer
        float layerDepth = 1.0 / numLayers;
        // depth of current layer
        float currentLayerDepth = 0.0;
        // the amount to shift the texture coordinates per layer (from vector P)
        vec2 P = viewdir.xy / viewdir.z * heightScale; 
        vec2 deltaTexCoords = P / numLayers;
        //get initial values
        vec2  currentTexCoords     = texCoords;
        float currentDepthMapValue = texture(Displacement, currentTexCoords).r;
        while(currentLayerDepth < currentDepthMapValue)
        {
            // shift texture coordinates along direction of P
            currentTexCoords -= deltaTexCoords;
            // get depthmap value at current texture coordinates
            currentDepthMapValue = texture(Displacement, currentTexCoords).r;  
            // get depth of next layer
            currentLayerDepth += layerDepth;  
        }
        // get texture coordinates before collision (reverse operations)
        vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
        // get depth after and before collision for linear interpolation
        float afterDepth  = currentDepthMapValue - currentLayerDepth;
        float beforeDepth = texture(Displacement, prevTexCoords).r - currentLayerDepth + layerDepth;
        // interpolation of texture coordinates
        float weight = afterDepth / (afterDepth - beforeDepth);
        vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
        return finalTexCoords;
    }

    void main(void)
    {
        vec2 Texcoords = fs_in.Texcoord;
        if(hasDisplacementMap > 0)
        {
            vec3 viewDir = transpose(fs_in.TBN) * normalize(fs_in.viewPos - fs_in.FragPos.xyz);
            Texcoords = ParallaxMapping(fs_in.Texcoord, viewDir);
            if(Texcoords.x > 1.0 || Texcoords.y > 1.0 || Texcoords.x < 0.0 || Texcoords.y < 0.0)
                discard;
        }
        
        vec3 unitNormal = hasNormalMap > 0
            ? normalize(fs_in.TBN * (texture(normalmap, Texcoords).rgb * 2.0 - vec3(1.0)))
            : normalize(fs_in.worldNormal);

        vec4 FinishedColor      = hasTexture             > 0 ? texture(texture0, Texcoords) * color      : color;
        float reflFactor        = hasReflectionMap       > 0 ? texture(reflectionmap, Texcoords).r       : reflectivity;
        float refrFactor        = hasRefractionMap       > 0 ? texture(refractionmap, Texcoords).r       : refractivity;
        float specularStrength  = hasSpecularMap         > 0 ? texture(specularmap, Texcoords).r         : specularity;
        float roughnessStrength = hasRoughnessMap        > 0 ? texture(roughnessmap, Texcoords).r        : roughness;
        float AOStrength        = hasAmbientOcclusionMap > 0 ? texture(ambientOcclusionmap, Texcoords).r : 1.0;

        int BONE_TO_CHECK = 1;
        for(int i = 0; i < 3; i++)
        {
            if(fs_in.jointIndices[i] == BONE_TO_CHECK)
            {
                float weight = fs_in.weights[i];
                if(weight >= 0.7)
                    FinishedColor = vec4(1.0, 0.0, 0.0, 0.0) * weight;
                else if(weight >= 0.4 && weight <= 0.6)
                    FinishedColor = vec4(0.0, 1.0, 0.0, 0.0) * weight;
                else if(weight >= 0.1)
                    FinishedColor = vec4(1.0, 1.0, 0.0, 0.0) * weight;

                break;
            }
        }

        //float linearizedDepth = (2.0 * near) / (far + near - gl_FragCoord.z * (far - near));
        //gl_FragDepth = linearizedDepth;
        gPosition   = fs_in.FragPos;
        gNormal     = vec4(unitNormal, 1.0);
        gAlbedoSpec = vec4(FinishedColor.rgb, 1);
        gObjectData = vec4(roughnessStrength, specularStrength, reflFactor, AOStrength);
    }
)";