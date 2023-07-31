#pragma once
#include "OpenGL/Renderable.h"
#include <GUI/Primitives/Box.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>

#include <OpenGL/Object3DInstance.h>

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