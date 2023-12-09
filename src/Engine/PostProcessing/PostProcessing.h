#pragma once
#include <Graphics/ShaderProgram.h>

namespace Gum {
namespace PostProcessing
{
    extern ShaderProgram *PostProcessingShader;
    extern unsigned int iVertexShaderID;
    extern void initShaders();
}}