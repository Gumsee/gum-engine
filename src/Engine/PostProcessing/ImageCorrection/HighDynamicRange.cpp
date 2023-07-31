#include "HighDynamicRange.h"
#include <System/MemoryManagement.h>
#include "HighDynamicRangeShader.h"
#include "../../Shaders/ShaderManager.h"

HighDynamicRange::HighDynamicRange(Box *canvas)
{
    init(canvas);
    pShader = nullptr;
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram();
        pShader->addShader(Gum::ShaderManager::getShader("PostProcessingShaderVert"));
        pShader->addShader(new Shader(HighDynamicRangeFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("HighDynamicRangeShader");

        //Textures
        pShader->addTexture("textureSampler", 0);
        pShader->addUniform("exposure");
    }
}


HighDynamicRange::~HighDynamicRange()
{
  pShader->removeShader(0);
  Gum::_delete(pShader);
}

void HighDynamicRange::render(Texture* texture, float exposure)
{
  pFramebuffer->bind();
  pShader->use();
  pShader->loadUniform("exposure", exposure);

  texture->bind();
  pRenderCanvas->renderCustom();
  texture->unbind();

  pShader->unuse();
  pFramebuffer->unbind();
}