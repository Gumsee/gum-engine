#include "BloomEffect.h"
#include <System/MemoryManagement.h>

BloomEffect::BloomEffect(Box* canvas)
{
	init(canvas);
    Gum::_delete(pFramebuffer);

    pBrightfilter = new Brightfilter(canvas);
    pContrastChanger = new ContrastChanger(canvas, 2.8f);
    pCombineTextures = new CombineTextures(canvas);
    pGaussianBlur = new GaussianBlur(canvas, 10);
}


BloomEffect::~BloomEffect()
{

}

Texture* BloomEffect::render(Texture* texture)
{
    Texture* bloom = pGaussianBlur->render(
        pContrastChanger->render(
            pBrightfilter->render(texture)
        )
    );

    pCombineTextures->setSecondTexture(bloom);
    return pCombineTextures->render(texture);
}