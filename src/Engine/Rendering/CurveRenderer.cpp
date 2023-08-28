#include "CurveRenderer.h"
#include "Camera.h"
#include "Graphics/Variables.h"
#include "Maths/MatrixFunctions.h"
#include "Maths/vec.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/VertexBufferObject.h"
#include "Renderer.h"
#include "../Shaders/ShaderManager.h"
#include "System/Output.h"
#include <string>

CurveRenderer::CurveRenderer(Curve* curve)
{
    pCurve = curve;
    this->iNumSegments = 40;

    pShader = Gum::ShaderManager::getShaderProgram("SimpleShader");
    pIDShader = Gum::ShaderManager::getShaderProgram("ThicklinesShader");

    pTransMatricesVBO = new VertexBufferObject<mat4>();
    pTransMatricesVBO->setData({mTransformation}, Gum::Graphics::DataState::STATIC);

    pVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::LINE_STRIP);
    pPointsBuffer = new VertexBufferObject<vec3>();
    pPointsBuffer->setData(pCurve->getData(), Gum::Graphics::DataState::STATIC);
    pVAO->addAttribute(pPointsBuffer, 0, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(vec3), 0);
    pVAO->addAttributeMat4(pTransMatricesVBO, 3, Gum::Graphics::Datatypes::FLOAT, 1);
    pVAO->setVertexCount(pCurve->getData().size());



    pControlVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::LINE_STRIP);
    pControlPointsBuffer = new VertexBufferObject<vec3>();
    pControlPointsBuffer->setData(pCurve->getControlpoints(), Gum::Graphics::DataState::STATIC);
    pControlVAO->addAttribute(pControlPointsBuffer, 0, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(vec3), 0);
    pControlVAO->addAttributeMat4(pTransMatricesVBO, 3, Gum::Graphics::Datatypes::FLOAT, 1);
    pControlVAO->setVertexCount(pCurve->numPoints());
}

CurveRenderer::~CurveRenderer()
{
    
}

void CurveRenderer::onProjectionUpdate()
{
    ShaderProgram* oldShader = ShaderProgram::getCurrentlyBoundShader();
    pShader->use();
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());

    pIDShader->use();
    pIDShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    pIDShader->loadUniform("viewportSize", (vec2)Renderer::getActiveRenderer()->getFramebuffer()->getSize());

    if(oldShader != nullptr)
        oldShader->use();
}

void CurveRenderer::onTransformUpdate()
{
    pTransMatricesVBO->setData({mTransformation}, Gum::Graphics::DataState::STATIC);
}

void CurveRenderer::prerender()
{
    //glPatchParameteri(GL_PATCH_VERTICES, 4);
    pShader->use();
    //pShader->loadUniform("segmentCount", iNumSegments);
    //pShader->loadUniform("stripCount", 1);
    pShader->loadUniform("color", vec4(1,0,1,1));
    pShader->loadUniform("transformationMatrix", mTransformation);
    pShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
}

void CurveRenderer::render()
{
    pVAO->bind();
    pVAO->render(1);
    pVAO->unbind();

    /*pShader->loadUniform("color", vec4(0,1,0,1));
    pControlVAO->bind();
    glDrawArrays(GL_LINE_STRIP, 0, pControlVAO->getRenderCount());
    pControlVAO->unbind();*/
}

void CurveRenderer::renderID()
{
    pIDShader->use();
    pIDShader->loadUniform("color", pCurve->getIndividualColor());
    pIDShader->loadUniform("transformationMatrix", mTransformation);
    pIDShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pVAO->bind();
    pVAO->render(1);
    pVAO->unbind();
    pIDShader->unuse();
}

void CurveRenderer::setSegments(int segments)
{
    this->iNumSegments = segments;
}

Curve* CurveRenderer::getCurve()
{
    return pCurve;
}