#pragma once
#include "Graphics/Renderable.h"
#include <GUI/Primitives/Box.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/Framebuffer.h>

#include <Graphics/Object3DInstance.h>

class OutlineRenderer
{
private:
    ShaderProgram* pShader;
    Box* pCanvas;

public:
    OutlineRenderer(Box* rendercanvas);
    ~OutlineRenderer();

    void render(Renderable* renderable, mat4 transform);
 
    void initShader();
};