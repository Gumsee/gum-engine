#include "AnimatedModel.h"
#include "Graphics/Object3DInstance.h"
#include "Primitives/SkeletalAnimation.h"
#include "RagdollObjectInstance.h"
#include <Essentials/Tools.h>
#include <Graphics/Variables.h>
#include <System/MemoryManagement.h>
#include <Codecs/Scene3DLoader.h>
#include <Graphics/ShaderProgram.h>
#include <vector>


AnimatedModel::AnimatedModel(const Gum::File& file, const std::string& name)
    : SceneObject(name)
{
	this->pShader = nullptr;
	//Combine all meshes into one

    if(Tools::mapHasKey(Mesh::mLoadedMeshes, file.toString()))
    {
        pMesh = Mesh::mLoadedMeshes[file.toString()];
    }
    else
    {
        pMesh = new Mesh(file.getName());
        Scene3DLoader loader;
        loader.iterateMeshes([this]([[maybe_unused]] unsigned int currentMesh, [[maybe_unused]] unsigned int numMeshes, Mesh* mesh, Bone* rootbone, std::vector<Bone*> bones) {
            pMesh->addMesh(mesh);
            Gum::_delete(mesh);            
            pSkeleton = new Skeleton(rootbone, (unsigned int)bones.size());
        });
        loader.iterateAnimations([this](SkeletalAnimation* anim) {
            pSkeleton->addAnimation(anim);
        });
        loader.load(file);
        Mesh::mLoadedMeshes[file.toString()] = pMesh;
    }

	load();

    pVertexArrayObject->bind();
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Winvalid-offsetof"
    pVertexArrayObject->addAttribute(pVertexVBO, 8, 4, Gum::Graphics::Datatypes::INTEGER, sizeof(Vertex), offsetof(Vertex, JointIDs));
    pVertexArrayObject->addAttribute(pVertexVBO, 9, 4, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, Weights));
    #pragma GCC diagnostic pop
    pVertexArrayObject->unbind();
}

AnimatedModel::~AnimatedModel() {}

void AnimatedModel::render()
{
	pSkeleton->update();
    for(Object3DInstance* instance : vInstances)
        instance->update();

    ShaderProgram* currentShader = ShaderProgram::getCurrentlyBoundShader();
    for(size_t i = 0; i < pSkeleton->getBoneMatrices().size(); i++)
        currentShader->loadUniform("gBones[" + std::to_string(i) + "]", pSkeleton->getBoneMatrices()[i]);

    //Load TextureObject related uniforms
    currentShader->loadUniform("isSkeletal", pSkeleton->getBoneMatrices().size() > 0);

    SceneObject::render();

    currentShader->loadUniform("isSkeletal", (int)false);
}

RagdollObjectInstance* AnimatedModel::addRagdollInstance(World3D* world)
{
    RagdollObjectInstance* instance = new RagdollObjectInstance(this, world);
    addInstance(instance);

    return instance;
}

//
// Getter
//
Skeleton* AnimatedModel::getSkeleton() { return pSkeleton; }