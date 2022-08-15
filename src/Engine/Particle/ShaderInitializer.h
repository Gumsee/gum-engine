#pragma once
#include <OpenGL/ShaderProgram.h>

namespace Gum {
namespace Particles 
{
    extern ShaderProgram* pParticleShader;
    extern ShaderProgram* pBillboardShader;
    
    extern void initShaders();
}}