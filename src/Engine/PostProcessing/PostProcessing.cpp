#include "PostProcessing.h"
#include "PostProcessingShader.h"

namespace Gum {
namespace PostProcessing
{
    Shader* VertexShader = nullptr;
    ShaderProgram* PostProcessingShader = nullptr;

    void initShaders()
    {
        if(PostProcessingShader == nullptr)
        {
            VertexShader = new Shader(PostProcessingVertexShader, Shader::TYPES::VERTEX_SHADER);
            Shader* PostProcessingShaderFrag = new Shader(PostProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER);

            PostProcessingShader = new ShaderProgram("PostProcessingShader", true);
            PostProcessingShader->addShader(VertexShader);
            PostProcessingShader->addShader(PostProcessingShaderFrag);
            PostProcessingShader->build();
        }
    }
}}