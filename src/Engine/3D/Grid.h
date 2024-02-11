#pragma once
#include "Graphics/Renderable.h"
#include "Graphics/VertexArrayObject.h"
#include <Graphics/ShaderProgram.h>

class Grid : public Renderable
{
private:
    static inline ShaderProgram* pShader = nullptr;
    VertexArrayObject* pVAO;

public:
    Grid();
    ~Grid();

    void render() override;
};