#pragma once
#include <Graphics/VertexArrayObject.h>
#include <Graphics/Texture.h>

class Canvas
{
private:
    VertexArrayObject* pVAO;
    ivec2 iSize;
    Texture* pTexture;

public:
    Canvas(const ivec2& size, Texture* texture = nullptr);
    void render();

    void setSize(const ivec2& size);
    void setTexture(Texture* texture);

    ivec2 getSize() const;
    Texture* getTexture();
};