#include "HighDynamicRange.h"
#include "HighDynamicRangeShader.h"
#include "../../Managers/ShaderManager.h"

HighDynamicRange::HighDynamicRange(Box *canvas, ivec2 resolution)
{
    init(canvas, resolution);
    if(Gum::ShaderManager::hasShaderProgram("HighDynamicRangeShader"))
    {
		pShader = Gum::ShaderManager::getShaderProgram("HighDynamicRangeShader");
    }
    else
    {
        pShader = new ShaderProgram();
        pShader->addShader(Gum::ShaderManager::getShader("PostProcessingShaderVert"));
        pShader->addShader(new Shader(HighDynamicRangeFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("HighDynamicRangeShader");

        //Textures
        pShader->addTexture("texture", 0);
        pShader->addUniform("exposure");

        Gum::ShaderManager::addShaderProgram(pShader);
    }
}


HighDynamicRange::~HighDynamicRange(){ }

void HighDynamicRange::render(Texture* texture, float exposure)
{
  pFramebuffer->bind();
  pShader->use();
  pShader->LoadUniform("exposure", exposure);

  texture->bind();
  pRenderCanvas->render();
  texture->unbind();

  pShader->unuse();
  pFramebuffer->unbind();
}