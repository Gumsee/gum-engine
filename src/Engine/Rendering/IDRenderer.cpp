#include "IDRenderer.h"
#include "IDRendererShader.h"

IDRenderer::IDRenderer(Box* canvas)
    : Framebuffer(canvas->getSize(), false)
{
    pCanvas = canvas;

    addTextureAttachment(0, "IndividualMap");

    initShaders();
}

IDRenderer::~IDRenderer()
{

}

ShaderProgram* IDRenderer::getMeshShader()
{
    return pMeshIDShader;
}

Texture2D* IDRenderer::getResultTexture()
{
    return (Texture2D*)getTextureAttachment();
}

void IDRenderer::initShaders()
{
    if(pMeshIDShader == nullptr)
    {
        pMeshIDShader = new ShaderProgram();
        pMeshIDShader->addShader(new Shader(MeshIDVertexShader, Shader::TYPES::VERTEX_SHADER));
        pMeshIDShader->addShader(new Shader(MeshIDFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pMeshIDShader->build("CurveIDShader", { {"vertices", 0}, {"TransMatrix", 3}, {"individualColor", 10} });
    }
}