#include "ShaderInitializer.h"
#include "../Shaders/ShaderManager.h"
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
            pParticleShader = new ShaderProgram(true);
            pParticleShader->addShader(new Shader(ParticleVertexShader, Shader::TYPES::VERTEX_SHADER));
            pParticleShader->addShader(new Shader(ParticleFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
            pParticleShader->build("ParticleShader", {
                {"vertexPosition", 0},
                {"partPositions", 1},
                {"partSizes", 2},
                {"texOffset", 3},
                {"TexCoordInfo", 4}
            });
            pParticleShader->addTexture("textureSampler", 0);
            Gum::ShaderManager::addShaderProgram(pParticleShader);
        }

        if(pBillboardShader == nullptr)
        {            
            pBillboardShader = new ShaderProgram(true);
            pBillboardShader->addShader(new Shader(BillboardVertexShader, Shader::TYPES::VERTEX_SHADER));
            pBillboardShader->addShader(new Shader(BillboardFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
            pBillboardShader->build("BillboardShader", {{"vertexPosition", 0}});
            pBillboardShader->addUniform("billboardCenter");
            pBillboardShader->addUniform("billboardSize");
            pBillboardShader->addUniform("fixedSize");
            pBillboardShader->addTexture("textureSampler", 0);
            Gum::ShaderManager::addShaderProgram(pBillboardShader);
        }
    }
}}