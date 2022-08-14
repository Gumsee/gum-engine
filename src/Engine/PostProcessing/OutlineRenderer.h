#pragma once
#include <GUI/Primitives/Box.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>

#include "../Object/Object.h"

class OutlineRenderer
{
private:
    ShaderProgram* pShader;
    Framebuffer* pFramebuffer;

public:
    OutlineRenderer(Box* rendercanvas);
    ~OutlineRenderer();

    void resetFramebuffer();

    void renderObject(Instance *instance);
    void renderOutlines(Instance *instance);

    void initShader();
    Framebuffer* getFramebuffer();
};