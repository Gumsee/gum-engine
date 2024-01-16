#include "IDRenderer.h"
#include "IDRendererShader.h"

IDRenderer::IDRenderer(Box* canvas)
    : Framebuffer(canvas->getSize(), false)
{
    pCanvas = canvas;

    addTextureAttachment(0, "IndividualMap");

    if(pMeshIDShader == nullptr)
    {
        pMeshIDShader = new ShaderProgram(true);
        pMeshIDShader->addShader(new Shader(MeshIDVertexShader, Shader::TYPES::VERTEX_SHADER));
        pMeshIDShader->addShader(new Shader(MeshIDFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pMeshIDShader->build("MeshIDShader", { {"vertices", 0}, {"TransMatrix", 3}, {"individualColor", 10} });
    }
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