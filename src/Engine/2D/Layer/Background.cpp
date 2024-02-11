#include "Background.h"
#include "Graphics/VertexArrayObject.h"

Background::Background(std::string name)
    : Layer(name, "background")
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

Background::~Background()
{

}

void Background::render()
{
    pShader->use();
    pVertexArrayObject->bind();
    pVertexArrayObject->renderIndexed(iNumLayers);
    pVertexArrayObject->unbind();
}

void Background::addLayer()
{
    iNumLayers++;
}