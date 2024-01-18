#include "BoneRenderer.h"
#include "BoneRendererShader.h"
#include "../../../../Rendering/Camera.h"
#include "Graphics/Object3D.h"
#include "Primitives/Mesh.h"
#include "System/Output.h"

BoneRenderer::BoneRenderer(AnimatedModel* model)
{
    this->pModel = model;

    Mesh* boneMesh = new Mesh();
    boneMesh->addVertices({
        vec3(-0.50, 0.00, -0.50),
        vec3(0.50, 0.00, 0.50),
        vec3(-0.50, 0.00, 0.50),
        vec3(0.50, 0.00, 0.50),
        vec3(0.50, 0.00, -0.50),
        vec3(0.00, 1.00, 0.00),
        vec3(-0.50, 0.00, -0.50),
        vec3(-0.50, 0.00, 0.50),
        vec3(0.00, 1.00, 0.00),
        vec3(0.50, 0.00, -0.50),
        vec3(-0.50, 0.00, -0.50),
        vec3(0.00, 1.00, 0.00),
        vec3(-0.50, 0.00, 0.50),
        vec3(0.50, 0.00, 0.50),
        vec3(0.00, 1.00, 0.00),
        vec3(0.50, 0.00, -0.50),
    });

    boneMesh->addIndices({
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 15, 1,
    });

    pBoneObject = new Object3D(boneMesh, "bone");
    for(size_t i = 0; i < pModel->getSkeleton()->getBoneMatrices().size(); i++)
        pBoneObject->addInstance();


    if(pShader == nullptr)
    {
        pShader = new ShaderProgram(true);
        pShader->addShader(new Shader(BoneRendererVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(BoneRendererFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("BoneRendererShader", { {"vertices", 0}, {"transMatrices", 3} });

        for(int i = 0; i < 100; i++)
            pShader->addUniform("bones[" + std::to_string(i) + "]");
    }


    for(size_t i = 0; i < pModel->getSkeleton()->getBoneMatrices().size(); i++)
    {
        pBoneObject->getInstance(i)->setMatrix(pModel->getSkeleton()->getBoneMatrices()[i]);
        pBoneObject->getInstance(i)->updateMatrix();
    }
}

BoneRenderer::~BoneRenderer()
{

}


void BoneRenderer::render()
{
    pShader->use();
    pShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    
    for(size_t i = 0; i < pModel->getSkeleton()->getBoneMatrices().size(); i++)
    {
        pBoneObject->getInstance(i)->setMatrix(pModel->getSkeleton()->getBoneMatrices()[i] * pModel->getInstance()->getMatrix());
        pBoneObject->getInstance(i)->updateMatrix();
    }
    
    pBoneObject->render();
    
    pShader->unuse();
}