#pragma once
#include <OpenGL/Shader.h>

static const std::string GBufferVertexShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) in vec3 vertexPosition;
    layout (location = 1) in vec2 TextureCoords;
    layout (location = 2) in vec3 Normals;
    layout (location = 3) in mat4 TransMatrix;
    layout (location = 7) in vec3 tangentNormals;
    layout (location = 8) in ivec3 jointIndices;
    layout (location = 9) in vec3 weights;
    layout (location = 10) in vec3 individualColor;
    out VS_OUT
    {
            vec2 Texcoord;
            vec3 worldNormal;
            vec4 FragPos;
            mat3 TBN;
            mat4 viewmat;
            vec3 viewPos;
            vec3 individualColor;
            float distance;
            vec3 weights;
            flat ivec3 jointIndices;
            flat int isSkel;
    } vs_out;

    flat out int frag_hasNormalMap;
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
    uniform int hasNormalMap;
    uniform vec4 ClipPlane;
    uniform vec3 viewPos;
    uniform int flipNormal;
    uniform int isInstanced;

    void main()
    {
        mat4 FinalTrans = transformationMatrix;
        //Is Instanced Check
        if(isInstanced == 1)
        {
            FinalTrans = TransMatrix;
        }
        //FinalTrans = mat4(1);
        vs_out.individualColor = individualColor;
        vs_out.Texcoord = TextureCoords * TextureMultiplier;
        vs_out.viewmat = viewMatrix;
        vec3 FinalVertexPosition = vertexPosition;
        vec4 totalLocalPos = vec4(0.0);
        //Is Skeletal Check
        vs_out.isSkel = isSkeletal;
        if(isSkeletal == 1)
        {
            vs_out.weights = weights;
            vs_out.jointIndices = jointIndices;
            for(int i=0;i<MAX_WEIGHTS;i++)
            {
                mat4 jointTransform = gBones[jointIndices[i]];
                vec4 posePosition = jointTransform * vec4(vertexPosition, 1.0);
                totalLocalPos += posePosition * weights[i];
            }
            FinalVertexPosition = totalLocalPos.xyz;
        }
        vec4 worldPosition = FinalTrans * vec4(FinalVertexPosition, 1.0f);
        vec4 positionRelativeToCam = projectionMatrix * viewMatrix * worldPosition;
        gl_ClipDistance[0] = dot(worldPosition, ClipPlane);
        //if(flipNormal > 0) { FinalNormal *= -1; }
        /*mat4 normalMatrix = transpose(inverse(FinalTrans));
        vec3 norm = normalize(vs_out.worldNormal.xyz);
        vec3 tang = normalize((normalMatrix * vec4(tangentNormals, 0.0)).xyz);
        vec3 bitang = normalize(cross(norm, tang));
        vs_out.TBN = transpose(mat3(
                tang.x, bitang.x, norm.x,
                tang.y, bitang.y, norm.y,
                tang.z, bitang.z, norm.z
        ));*/

        frag_hasNormalMap = hasNormalMap;

        //vs_out.worldNormal = (normalMatrix * vec4(FinalNormal, 0.0)).xyz;// + vec4(0.000001);
        vs_out.worldNormal = (viewMatrix * FinalTrans * vec4(Normals, 0.0f)).xyz;
        vs_out.FragPos = viewMatrix * worldPosition;
        vs_out.viewPos = viewPos;
        vs_out.distance = positionRelativeToCam.z;
        gl_Position = positionRelativeToCam;
    }
)";


static const std::string GBufferGeometryShader = Shader::SHADER_VERSION_STR + 
R"(
    
)";


static const std::string GBufferFragmentShader = Shader::SHADER_VERSION_STR + 
R"(
    layout (location = 0) out vec4 gPosition;
    layout (location = 1) out vec4 gIndividualColor;
    layout (location = 2) out vec4 gNormal;
    layout (location = 3) out vec4 gAlbedoSpec;
    layout (location = 4) out vec4 gObjectData;

    in VS_OUT
    {
            vec2 Texcoord;
            vec3 worldNormal;
            vec4 FragPos;
            mat3 TBN;
            mat4 viewmat;
            vec3 viewPos;
            vec3 individualColor;
            float distance;
            vec3 weights;
            flat ivec3 jointIndices;
            flat int isSkel;
    } fs_in;

    flat in int frag_hasNormalMap;

    uniform sampler2D texture0;
    uniform samplerCube Enviorment;
    uniform sampler2D normalmap;
    uniform sampler2D specularmap;
    uniform sampler2D reflectionmap;
    uniform sampler2D refractionmap;
    uniform sampler2D roughnessmap;
    uniform sampler2D ambientOcclusionmap;
    uniform sampler2D Displacement;

    uniform vec3 SunColor;
    uniform vec3 lightColor[4];
    uniform vec3 lightAttenuation[4];
    uniform vec4 color;

    uniform int hasTexture;
    uniform int hasSpecularMap;
    uniform int hasReflectionMap;
    uniform int hasRefractionMap;
    uniform int hasRoughnessMap;
    uniform int hasAmbientOcclusionMap;
    uniform int hasDisplacementMap;

    uniform float specularFactor;
    uniform float ReflectionFactor;
    uniform float RefractionFactor;
    uniform float roughnessFactor;

    vec3 unitNormal;
    vec3 unitToCameraVector; 

    float near = 0.1;
    float far = 5000.0; 

    float heightScale = 0.1;

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
        if(hasDisplacementMap == 1)
        {
            vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos.xyz);
            Texcoords = ParallaxMapping(fs_in.Texcoord, viewDir);
            //if(Texcoords.x > 1.0 || Texcoords.y > 1.0 || Texcoords.x < 0.0 || Texcoords.y < 0.0)
    //      discard;
        }
        if(frag_hasNormalMap == 1)
        {
            //Only apply if existing
            vec3 normalMapValue = texture(normalmap, Texcoords).rgb;
            normalMapValue = normalize(normalMapValue * 2.0 - vec3(1.0));
            unitNormal = normalMapValue; //JUST FOR 
            //unitNormal = fs_in.TBN * normalize(normalMapValue);
        }
        else
        {
            unitNormal = normalize(fs_in.worldNormal);
            //unitNormal = fs_in.worldNormal;
        }
        vec4 FinishedColor = color;
        if(hasTexture > 0)
            FinishedColor = texture2D(texture0, Texcoords) * color;

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

        float reflFactor = ReflectionFactor;
        if(hasReflectionMap > 0) { reflFactor = texture(reflectionmap, Texcoords).r; }
        float refrFactor = RefractionFactor;
        if(hasRefractionMap > 0) { refrFactor = texture(refractionmap, Texcoords).r; }
        float specularStrength = specularFactor;
        if(hasSpecularMap > 0) { specularStrength = texture(specularmap, Texcoords).r; }
        float roughnessStrength = roughnessFactor;
        if(hasRoughnessMap > 0) { roughnessStrength = texture(roughnessmap, Texcoords).r; }
        float AOStrength = 1.0;
        if(hasAmbientOcclusionMap > 0) { AOStrength = texture(ambientOcclusionmap, Texcoords).r; }
        //float linearizedDepth = (2.0 * near) / (far + near - gl_FragCoord.z * (far - near));  
        //gl_FragDepth = fs_in.FragPos.z;
        gPosition =             fs_in.FragPos;
        gIndividualColor =      vec4(fs_in.individualColor, 1.0f);
        gNormal =               vec4(unitNormal, 1.0f); //vec4(fs_in.worldNormal, 1.0f);
        gAlbedoSpec =           vec4(FinishedColor.rgb, 1);
        gObjectData =           vec4(roughnessStrength,specularStrength,reflFactor,AOStrength);
    }
)";