#pragma once
#include <Graphics/ShaderProgram.h>

namespace Gum {
namespace Particles 
{
    extern ShaderProgram* pParticleShader;
    extern ShaderProgram* pBillboardShader;
    
    extern void initShaders();
}}