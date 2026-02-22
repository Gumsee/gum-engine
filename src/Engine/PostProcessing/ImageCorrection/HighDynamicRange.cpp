#include "HighDynamicRange.h"
#include <System/MemoryManagement.h>
#include "HighDynamicRangeShader.h"
#include "../PostProcessing.h"

HighDynamicRange::HighDynamicRange(Box *canvas)
{
    init(canvas);
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram("HighDynamicRangeShader", true);
        pShader->addShader(Gum::PostProcessing::VertexShader);
        pShader->addShader(new Shader(HighDynamicRangeFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build();

        //Textures
        pShader->addTexture("textureSampler", 0);
    }
}


HighDynamicRange::~HighDynamicRange()
{
    pShader->removeShader(0);
    Gum::_delete(pShader);
}

Texture* HighDynamicRange::render(Texture* texture, float exposure)
{
    pFramebuffer->bind();
    pShader->use();
    pShader->loadUniform("exposure", exposure);

    texture->bind();
    pRenderCanvas->renderCustom();
    texture->unbind();

    pShader->unuse();
    pFramebuffer->unbind();

    return pFramebuffer->getTextureAttachment(0);
}