#include "AnimatedModel.h"
#include "Primitives/SkeletalAnimation.h"
#include <Essentials/Tools.h>
#include <Graphics/Variables.h>
#include <System/MemoryManagement.h>
#include <Codecs/Scene3DLoader.h>
#include <Graphics/ShaderProgram.h>
#include <vector>


AnimatedModel::AnimatedModel(std::string file, std::string name)
    : SceneObject(name)
{
	this->pShader = nullptr;
	//Combine all meshes into one

    if(Tools::mapHasKey(Mesh::mLoadedMeshes, file))
    {
        pMesh = Mesh::mLoadedMeshes[file];
    }
    else
    {
        pMesh = new Mesh();
        pMesh->name = file;
        Scene3DLoader loader;
        loader.iterateMeshes([this](unsigned int currentMesh, unsigned int numMeshes, Mesh* mesh, Bone* rootbone, std::vector<Bone*> bones) {
            pMesh->addMesh(mesh);
            Gum::_delete(mesh);            
            pSkeleton = new Skeleton(rootbone, bones.size());
        });
        loader.iterateAnimations([this](SkeletalAnimation* anim) {
            pSkeleton->addAnimation(anim);
        });
        loader.load(file);
        Mesh::mLoadedMeshes[file] = pMesh;
    }

	load();

    pVertexArrayObject->bind();
    pVertexArrayObject->addAttribute(pVertexVBO, 8, 4, Gum::Graphics::Datatypes::INTEGER, sizeof(Vertex), offsetof(Vertex, JointIDs));
    pVertexArrayObject->addAttribute(pVertexVBO, 9, 4, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, Weights));
    pVertexArrayObject->unbind();
}

AnimatedModel::~AnimatedModel() {}

void AnimatedModel::render()
{
    ShaderProgram* currentShader = ShaderProgram::getCurrentlyBoundShader();
    for(size_t i = 0; i < pSkeleton->getBoneMatrices().size(); i++)
        currentShader->loadUniform("gBones[" + std::to_string(i) + "]", pSkeleton->getBoneMatrices()[i]);

    //Load TextureObject related uniforms
    currentShader->loadUniform("isSkeletal", pSkeleton->getBoneMatrices().size() > 0);

    SceneObject::render();

    currentShader->loadUniform("isSkeletal", (int)false);


	pSkeleton->update();
}

//
// Getter
//
Skeleton* AnimatedModel::getSkeleton() { return pSkeleton; }