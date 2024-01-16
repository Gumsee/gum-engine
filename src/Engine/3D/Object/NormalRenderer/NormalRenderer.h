#pragma once
#include "Graphics/Renderable.h"
#include "../../World3D.h"

class NormalRenderer : public Renderable
{
private:
    World3D* pWorld;

    inline static ShaderProgram* pShader = nullptr;

    float fNormalLength;

public:
    NormalRenderer(World3D* world, const float& normallength = 0.4f);
    ~NormalRenderer();

    void render() override;
};