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
    VertexShader = new Shader(PostProcessingVertexShader, Shader::TYPES::VERTEX_SHADER);
    FragmentShader = new Shader(PostProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER);
    Shader* PostProcessingShaderFrag = new Shader(PostProcessingFragmentShader, Shader::TYPES::FRAGMENT_SHADER);

    PostProcessingShader = ShaderProgram::requestShaderProgram("PostProcessingShader", true);
    PostProcessingShader->addShader(VertexShader);
    PostProcessingShader->addShader(PostProcessingShaderFrag);
    PostProcessingShader->build();
  }
}}