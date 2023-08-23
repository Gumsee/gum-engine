#pragma once
#include "Graphics/ShaderProgram.h"
#include "Graphics/VertexArrayObject.h"
#include <Graphics/Renderable.h>
#include <Primitives/Transformable.h>

class Gizmo : public Transformable<3U>, public Renderable
{
private:
    VertexArrayObject* pPositionVAO;
    inline static ShaderProgram* pShader = nullptr;

    static void initShader();

public:
    Gizmo();
    ~Gizmo();

    void onProjectionUpdate() override;

    void render() override;
    void renderID() override;
};