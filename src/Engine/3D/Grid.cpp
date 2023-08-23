#include "Grid.h"
#include "GridShader.h"
#include "Camera3D.h"
#include "../Shaders/ShaderManager.h"
#include "Graphics/VertexArrayObject.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"

Grid::Grid()
{
    pVAO = new VertexArrayObject();
    VertexBufferObject<float> vbo;
    vbo.setData({}); //TODO
    pVAO->addAttribute(&vbo, 0, 2, GL_FLOAT);
    initShader();
}

Grid::~Grid()
{
    Gum::_delete(pVAO);
}

void Grid::render()
{
    pShader->use();
    pShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());

    glEnable(GL_BLEND);
    pVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    pVAO->unbind();
    glDisable(GL_BLEND);
    pShader->unuse();
}

void Grid::initShader()
{
    if(Gum::ShaderManager::hasShaderProgram("GridShader"))
    {
		pShader = Gum::ShaderManager::getShaderProgram("GridShader");
    }
    else
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(GridVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(GridFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("GridShader");



        Gum::ShaderManager::addShaderProgram(pShader);
    }
}