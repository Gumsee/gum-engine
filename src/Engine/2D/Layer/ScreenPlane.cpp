#include "ScreenPlane.h"
#include "../../Shaders/ShaderManager.h"
#include "Graphics/VertexArrayObject.h"

ScreenPlane::ScreenPlane()
{
    iNumLayers = 1U;

    if(pVertexArrayObject == nullptr)
    {
        pVertexArrayObject = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLES);

        pVertexArrayObject->bind();
        VertexBufferObject<float> pVertexVBO;
		pVertexVBO.setData(afSkyVertices, Gum::Graphics::DataState::STATIC);
        pVertexArrayObject->addAttribute(&pVertexVBO, 0, 2, Gum::Graphics::Datatypes::FLOAT, 0, 0);
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
    pVertexArrayObject->renderIndexed(iNumLayers);
    pVertexArrayObject->unbind();
}

void ScreenPlane::addLayer()
{
    iNumLayers++;
}