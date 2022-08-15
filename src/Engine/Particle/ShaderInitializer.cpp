#include "ShaderInitializer.h"
#include "../Managers/ShaderManager.h"
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
            pParticleShader = new ShaderProgram();
            pParticleShader->addShader(new Shader(ParticleVertexShader, Shader::VERTEX_SHADER));
            pParticleShader->addShader(new Shader(ParticleFragmentShader, Shader::FRAGMENT_SHADER));
            pParticleShader->build("ParticleShader");
            Gum::ShaderManager::addShaderProgram(pParticleShader);
        }

        if(pBillboardShader == nullptr)
        {            
            pBillboardShader = new ShaderProgram();
            pBillboardShader->addShader(new Shader(BillboardVertexShader, Shader::VERTEX_SHADER));
            pBillboardShader->addShader(new Shader(BillboardFragmentShader, Shader::FRAGMENT_SHADER));
            pBillboardShader->build("BillboardShader");
            Gum::ShaderManager::addShaderProgram(pBillboardShader);
        }
    }
}}