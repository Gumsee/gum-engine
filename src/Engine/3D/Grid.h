#pragma once
#include "OpenGL/Renderable.h"
#include "OpenGL/VertexArrayObject.h"
#include <OpenGL/ShaderProgram.h>

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