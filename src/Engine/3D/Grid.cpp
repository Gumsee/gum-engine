#include "Grid.h"
#include "GridShader.h"
#include "Camera3D.h"
#include <Graphics/VertexArrayObject.h>
#include <Graphics/Graphics.h>
#include <System/MemoryManagement.h>
#include <System/Output.h>

Grid::Grid()
{
    pVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLES);
    VertexBufferObject<float> vbo;
    vbo.setData({}, Gum::Graphics::DataState::STATIC); //TODO
    pVAO->addAttribute(&vbo, 0, 2, Gum::Graphics::Datatypes::FLOAT);
    pVAO->setVertexCount(6);
    
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram("GridShader", true);
        pShader->addShader(new Shader(GridVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(GridFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build();
    }
}

Grid::~Grid()
{
    Gum::_delete(pVAO);
}

void Grid::render()
{
    pShader->use();
    Gum::Graphics::disableFeature(Gum::Graphics::Features::CULL_FACE);
    Gum::Graphics::enableFeature(Gum::Graphics::Features::BLENDING);
    pVAO->bind();
    pVAO->render(1);
    pVAO->unbind();
    pShader->unuse();
    Gum::Graphics::enableFeature(Gum::Graphics::Features::CULL_FACE);
}