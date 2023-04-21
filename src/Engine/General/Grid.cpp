#include "Grid.h"
#include "GridShader.h"
#include "Camera.h"
#include "../Managers/ShaderManager.h"

Grid::Grid()
{
    initShader();
}

Grid::~Grid()
{

}

void Grid::render()
{
    pShader->use();
    pShader->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
    pShader->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());

    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
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
        pShader->addShader(new Shader(GridVertexShader, Shader::VERTEX_SHADER));
        pShader->addShader(new Shader(GridFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("GridShader");



        Gum::ShaderManager::addShaderProgram(pShader);
    }
}