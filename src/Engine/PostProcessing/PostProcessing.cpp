#include "PostProcessing.h"
#include "PostProcessingShader.h"

namespace Gum {
namespace PostProcessing
{
  Shader* VertexShader = nullptr;
  Shader* FragmentShader = nullptr;
  ShaderProgram* PostProcessingShader = nullptr;

  void initShaders()
  {
    VertexShader = Shader::requestShader("PostProcessingShader", PostProcessingVertexShader, Shader::TYPES::VERTEX_SHADER);
    FragmentShader = Shader::requestShader("PostProcessingShader", PostProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER);

    PostProcessingShader = ShaderProgram::requestShaderProgram("PostProcessingShader", true);
    PostProcessingShader->addShader(VertexShader);
    PostProcessingShader->addShader(FragmentShader);
    PostProcessingShader->build();
  }
}}