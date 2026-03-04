#pragma once
#include <Graphics/ShaderProgram.h>
#include <Graphics/Framebuffer.h>
#include "Canvas.h"

class IDRenderer : private Framebuffer
{
private:
    inline static ShaderProgram* pMeshIDShader = nullptr;

    Canvas* pCanvas;

public:
    IDRenderer(Canvas* canvas);
    ~IDRenderer();

    using Framebuffer::bind;
    using Framebuffer::setSize;
    using Framebuffer::getPixel;
    using Framebuffer::clear;

    ShaderProgram* getMeshShader();
    ShaderProgram* getCurveShader();
    Texture2D* getResultTexture();
};