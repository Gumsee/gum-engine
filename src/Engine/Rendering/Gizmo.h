#pragma once
#include <GUI/ShaderCanvas/ShaderCanvas.h>
#include "Graphics/ShaderProgram.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/VertexBufferObject.h"
#include <Graphics/Renderable.h>
#include <Primitives/Transformable.h>

class Gizmo : public Renderable, public Transformable<3U>
{
private:
    static inline ShaderProgram* pShader = nullptr;
    VertexArrayObject* pLineVAO;
    VertexBufferObject<vec2>* pLineVBO;
    std::vector<vec2> vLineVBOData;

    void addCircle(vec2 at, float radius);
    void addLine(vec2 from, vec2 to);

    void drawTranslationGizmo(int type);

public:
    Gizmo();
    ~Gizmo();

    void onProjectionUpdate() override;

    void render() override;
    void renderID() override;
};