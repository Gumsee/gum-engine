#pragma once
#include "../PostProcessingEffect.h"
#include "../Brightfilter/Brightfilter.h"
#include "../ContrastChanger/ContrastChanger.h"
#include "../CombineTextures/CombineTextures.h"
#include "../GaussianBlur/GaussianBlur.h"

class BloomEffect : public PostProcessingEffect
{
private:
    Brightfilter *pBrightfilter;
    ContrastChanger *pContrastChanger;
    CombineTextures *pCombineTextures;
    GaussianBlur *pGaussianBlur;

public:
	BloomEffect(Box* canvas);
	~BloomEffect();

	Texture* render(Texture* texture);
};