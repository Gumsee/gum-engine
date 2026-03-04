#pragma once
#include <Graphics/ShaderProgram.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/Object3DInstance.h>
#include <Graphics/Renderable.h>
#include "../../Rendering/Canvas.h"

class OutlineRenderer
{
private:
    ShaderProgram* pShader;
    ShaderProgram* pProcessingShader;
    Canvas* pCanvas;
    Framebuffer* pFramebuffer;
    color cColor;

public:
    OutlineRenderer(Canvas* rendercanvas);
    ~OutlineRenderer();

    void render(Renderable* renderable, mat4 transform);
 
    void initShader();
    void updateSize();

    void setColor(const color& col);
    color getColor() const;
};