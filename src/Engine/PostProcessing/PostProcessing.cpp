#include "PostProcessing.h"
#include "PostProcessingShader.h"
#include "../Shaders/ShaderManager.h"

namespace Gum {
namespace PostProcessing
{
    unsigned int iVertexShaderID = 0;
    ShaderProgram* PostProcessingShader = nullptr;

    void initShaders()
    {
        if(PostProcessingShader == nullptr)
        {
            Shader* PostProcessingShaderVert = new Shader(PostProcessingVertexShader, Shader::TYPES::VERTEX_SHADER);
            Shader* PostProcessingShaderFrag = new Shader(PostProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER);

            PostProcessingShader = new ShaderProgram(true);
            PostProcessingShader->addShader(PostProcessingShaderVert);
            PostProcessingShader->addShader(PostProcessingShaderFrag);
            PostProcessingShader->build("PostProcessingShader");
            Gum::ShaderManager::addShaderProgram(PostProcessingShader);
            iVertexShaderID = Gum::ShaderManager::addShader(PostProcessingShaderVert);
        }
    }
}}