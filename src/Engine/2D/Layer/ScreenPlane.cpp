#include "ScreenPlane.h"
#include "../../Shaders/ShaderManager.h"

ScreenPlane::ScreenPlane()
{
    iNumLayers = 1U;

    if(pVertexArrayObject == nullptr)
    {
        pVertexArrayObject = new VertexArrayObject();

        pVertexArrayObject->bind();
        VertexBufferObject<float> pVertexVBO;
		pVertexVBO.setData(afSkyVertices);
        pVertexArrayObject->addAttribute(&pVertexVBO, 0, 2, GL_FLOAT, 0, 0);
        pVertexArrayObject->setVertexCount(pVertexVBO.getLength());

        ElementBufferObject *indexBuffer = new ElementBufferObject();
        indexBuffer->setData(aiSkyIndices);
        pVertexArrayObject->addElementBuffer(indexBuffer);
        pVertexArrayObject->unbind();
    }
}

ScreenPlane::~ScreenPlane()
{

}

void ScreenPlane::render()
{
    pParallaxSkyShader->use();
    pVertexArrayObject->bind();
    glDrawElementsInstanced(pVertexArrayObject->getPrimitiveType(), pVertexArrayObject->getRenderCount(), GL_UNSIGNED_INT, 0, iNumLayers);
    pVertexArrayObject->unbind();
}

void ScreenPlane::addLayer()
{
    iNumLayers++;
}