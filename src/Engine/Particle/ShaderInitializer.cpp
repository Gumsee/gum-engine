#include "ShaderInitializer.h"
#include "ParticleShader.h"
#include "BillboardShader.h"

namespace Gum {
namespace Particles 
{
    ShaderProgram* pParticleShader = nullptr;
    ShaderProgram* pBillboardShader = nullptr;

    void initShaders()
    {
        if(pParticleShader == nullptr)
        {
            pParticleShader = new ShaderProgram("ParticleShader", true);
            pParticleShader->addShader(new Shader(ParticleVertexShader, Shader::TYPES::VERTEX_SHADER));
            pParticleShader->addShader(new Shader(ParticleFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
            pParticleShader->build({
                {"vertexPosition", 0},
                {"partPositions", 1},
                {"partSizes", 2},
                {"texOffset", 3},
                {"TexCoordInfo", 4}
            });
            pParticleShader->addTexture("textureSampler", 0);
        }

        if(pBillboardShader == nullptr)
        {            
            pBillboardShader = new ShaderProgram("BillboardShader", true);
            pBillboardShader->addShader(new Shader(BillboardVertexShader, Shader::TYPES::VERTEX_SHADER));
            pBillboardShader->addShader(new Shader(BillboardFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
            pBillboardShader->build({{"vertexPosition", 0}});
            pBillboardShader->addUniform("billboardCenter");
            pBillboardShader->addUniform("billboardSize");
            pBillboardShader->addUniform("fixedSize");
            pBillboardShader->addTexture("textureSampler", 0);
        }
    }
}}