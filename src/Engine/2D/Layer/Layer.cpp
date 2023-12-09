#include "Layer.h"
#include "DefaultLayerShader.h"

Layer::Layer(std::string name, std::string type) : sName(name), sType(type)
{
    pShader = initDefaultLayerShader();
}