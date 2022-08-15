#include "PostProcessing.h"
#include "PostProcessingShader.h"
#include "../Managers/ShaderManager.h"

namespace Gum {
namespace PostProcessing
{
    ShaderProgram* PostProcessingShader = nullptr;

    void initShaders()
    {
        if(PostProcessingShader == nullptr)
        {
            Shader* PostProcessingShaderVert = new Shader(PostProcessingVertexShader, Shader::TYPES::VERTEX_SHADER);
            Shader* PostProcessingShaderFrag = new Shader(PostProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER);

            PostProcessingShader = new ShaderProgram();
            PostProcessingShader->addShader(PostProcessingShaderVert);
            PostProcessingShader->addShader(PostProcessingShaderFrag);
            PostProcessingShader->build("PostProcessingShader");
            Gum::ShaderManager::addShaderProgram(PostProcessingShader);
            Gum::ShaderManager::addShader(PostProcessingShaderVert, "PostProcessingShaderVert");
        }
    }
}}