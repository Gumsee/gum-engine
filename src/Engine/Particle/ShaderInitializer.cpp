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
      pParticleShader = ShaderProgram::requestShaderProgram("ParticleShader", true);
      pParticleShader->addShader(Shader::requestShader("ParticleShader", ParticleVertexShader, Shader::TYPES::VERTEX_SHADER));
      pParticleShader->addShader(Shader::requestShader("ParticleShader", ParticleFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
      pParticleShader->build({
          {"vertexPosition", 0},
          {"partPositions", 1},
          {"partSizes", 2},
          {"texOffset", 3},
          {"TexCoordInfo", 4}
      });
      pParticleShader->addTexture("textureSampler", 0);
    
      pBillboardShader = ShaderProgram::requestShaderProgram("BillboardShader", true);
      pBillboardShader->addShader(Shader::requestShader("BillboardShader", BillboardVertexShader, Shader::TYPES::VERTEX_SHADER));
      pBillboardShader->addShader(Shader::requestShader("BillboardShader", BillboardFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
      pBillboardShader->build({{"vertexPosition", 0}});
      pBillboardShader->addTexture("textureSampler", 0);
    }
}}