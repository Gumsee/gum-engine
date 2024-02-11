#pragma once
#include <Graphics/ShaderProgram.h>

namespace Gum {
namespace PostProcessing
{
    extern ShaderProgram *PostProcessingShader;
    extern Shader* VertexShader;
    extern void initShaders();
}}