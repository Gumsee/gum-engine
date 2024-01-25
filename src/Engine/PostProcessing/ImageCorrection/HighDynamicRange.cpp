#include "HighDynamicRange.h"
#include <System/MemoryManagement.h>
#include "HighDynamicRangeShader.h"
#include "../../Shaders/ShaderManager.h"
#include "../PostProcessing.h"

HighDynamicRange::HighDynamicRange(Box *canvas)
{
    init(canvas);
    pShader = nullptr;
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram(true);
        pShader->addShader(Gum::ShaderManager::getShader(Gum::PostProcessing::iVertexShaderID));
        pShader->addShader(new Shader(HighDynamicRangeFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
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