#pragma once
#include <Graphics/ShaderProgram.h>
#include <Graphics/Framebuffer.h>
#include <GUI/Primitives/Box.h>

class IDRenderer : private Framebuffer
{
private:
    inline static ShaderProgram* pMeshIDShader = nullptr;
    static void initShaders();

    Box* pCanvas;

public:
    IDRenderer(Box* canvas);
    ~IDRenderer();

    using Framebuffer::bind;
    using Framebuffer::setSize;
    using Framebuffer::getPixel;
    using Framebuffer::clear;

    ShaderProgram* getMeshShader();
    ShaderProgram* getCurveShader();
    Texture2D* getResultTexture();
};