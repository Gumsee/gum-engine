#include "Gizmo.h"
#include "GizmoShader.h"
#include "Camera.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/VertexBufferObject.h"
#include "System/Output.h"

Gizmo::Gizmo()
{
    pPositionVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::LINE_STRIP);
    VertexBufferObject<mat4> *pTransMatricesVBO = new VertexBufferObject<mat4>();
    pTransMatricesVBO->setData({mTransformation});

    VertexBufferObject<vec3> *pColorsVBO = new VertexBufferObject<vec3>();
    pColorsVBO->setData({vec3(1,0,1), vec3(0,1,0)});

    VertexBufferObject<vec3> *pVertexVBO = new VertexBufferObject<vec3>();
    pVertexVBO->setData({vec3(1,1,1), vec3(2,2,2)});

    pPositionVAO->addAttribute(pVertexVBO, 0, 3, GL_FLOAT, sizeof(vec3), 0);
    pPositionVAO->addAttribute(pColorsVBO, 1, 3, GL_FLOAT, sizeof(vec3), 0);
    pPositionVAO->addAttributeMat4(pTransMatricesVBO, 2, GL_FLOAT, 1);
    pPositionVAO->setVertexCount(2);

    initShader();
}

Gizmo::~Gizmo()
{

}

void Gizmo::onProjectionUpdate()
{
    ShaderProgram* oldShader = ShaderProgram::getCurrentlyBoundShader();
    pShader->use();
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());

    if(oldShader != nullptr)
        oldShader->use();
}

void Gizmo::render()
{
    pShader->use();
    pShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pPositionVAO->bind();
    glDrawArrays(GL_LINE_STRIP, 0, pPositionVAO->getRenderCount());
    pPositionVAO->unbind();
}

void Gizmo::renderID()
{

}

void Gizmo::initShader()
{
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(GizmoVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(GizmoFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("GizmoShader", { {"vertices", 0}, {"colors", 1}, {"TransMatrix", 2} });
    }
}