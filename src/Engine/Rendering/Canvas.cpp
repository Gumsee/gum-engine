#include "Canvas.h"

Canvas::Canvas(const ivec2& size, Texture* texture)
  : iSize(size),
    pTexture(texture)
{
    VertexBufferObject<float>* vbo = new VertexBufferObject<float>();
    vbo->setData({
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    }, Gum::Graphics::DataState::STATIC);

    ElementBufferObject* ebo = new ElementBufferObject();
    ebo->setData({0, 1, 2, 2, 3, 0});

    pVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLES);
    
    pVAO->setVertexCount(4);
    pVAO->addAttribute(vbo, 0, 2, Gum::Graphics::Datatypes::FLOAT, 0, 0);
    pVAO->addElementBuffer(ebo);
}

void Canvas::render()
{
    pVAO->bind();
    pVAO->renderIndexed(1);
    pVAO->unbind();
}


void Canvas::setSize(const ivec2& size)
{
    this->iSize = size;
}

void Canvas::setTexture(Texture* texture)
{
    this->pTexture = texture;
}


ivec2 Canvas::getSize() const
{
    return iSize;
}

Texture* Canvas::getTexture()
{
    return pTexture;
}