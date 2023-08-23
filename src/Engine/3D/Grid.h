#pragma once
#include "Graphics/Renderable.h"
#include "Graphics/VertexArrayObject.h"
#include <Graphics/ShaderProgram.h>

class Grid : public Renderable
{
private:
    ShaderProgram* pShader;
    VertexArrayObject* pVAO;
    void initShader();

public:
    Grid();
    ~Grid();

    void render() override;
};