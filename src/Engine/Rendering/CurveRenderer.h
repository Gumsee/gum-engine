#pragma once
#include <Primitives/Transformable.h>
#include <Primitives/Curve.h>
#include <Primitives/UniqueID.h>
#include <Graphics/Renderable.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/VertexArrayObject.h>
#include <Graphics/VertexBufferObject.h>

class CurveRenderer : public Transformable<3U>, public Renderable
{
private:
    inline static ShaderProgram *pShader = nullptr;
    inline static ShaderProgram* pIDShader = nullptr;
    
    Curve* pCurve;
    VertexArrayObject* pVAO;
    VertexBufferObject<vec3> *pPointsBuffer;
    VertexBufferObject<mat4> *pTransMatricesVBO;

    VertexArrayObject* pControlVAO;
    VertexBufferObject<vec3> *pControlPointsBuffer;
    int iNumSegments;

public:
    CurveRenderer(Curve* curve);
    ~CurveRenderer();

    void onProjectionUpdate() override;
    void onTransformUpdate() override;

    void prerender() override;
    void render() override;
    void renderID() override;

    void setSegments(int segments);

    Curve* getCurve();
};