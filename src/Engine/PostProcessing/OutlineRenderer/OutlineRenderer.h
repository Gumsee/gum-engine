#pragma once
#include <GUI/Primitives/Box.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>

#include "../../Object/Object.h"

class OutlineRenderer
{
private:
    ShaderProgram* pShader, *pProcessingShader;
    Framebuffer* pFramebuffer, *pProcessingFramebuffer;
    Box* pCanvas;

public:
    OutlineRenderer(Box* rendercanvas);
    ~OutlineRenderer();

    void render(Instance *instance, Framebuffer* fb);
 
    void initShader();
    Framebuffer* getFramebuffer();
    Texture2D* getResultTexture();
};