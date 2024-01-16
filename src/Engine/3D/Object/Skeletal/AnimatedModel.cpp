#include "AnimatedModel.h"
#include <Essentials/Tools.h>
#include "../ObjectManager.h"
#include "Graphics/Variables.h"
#include <Codecs/Scene3DLoader.h>
#include <Graphics/ShaderProgram.h>
#include <vector>


AnimatedModel::AnimatedModel(std::string file, std::string name)
{
	this->pShader = nullptr;
	//Combine all meshes into one
	sName = name;
    pMaterial = new Material();

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

            for(size_t i = 0; i < bones.size(); i++)
            {
                if(bones[i]->getParent() != nullptr)
                    std::cout << bones[i]->getName() << " " << bones[i]->getParent()->getName() << std::endl;
            }
            
            mat4 globalInverseTransform; //TODO
            pSkeleton = new Skeleton(rootbone, globalInverseTransform);
        });
        loader.load(file);
        Mesh::mLoadedMeshes[file] = pMesh;
    }

	load();

    pVertexArrayObject->addAttribute(pVertexVBO, 8, 3, Gum::Graphics::Datatypes::INTEGER, sizeof(Vertex), offsetof(Vertex, JointIDs));
    pVertexArrayObject->addAttribute(pVertexVBO, 9, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, Weights));

    



    SkeletalAnimation* PlaceHolder = new SkeletalAnimation("Null", vec2(1, 1), 0, 0, 0);
	PlaySkeletalAnimation(PlaceHolder,false,true);
    StopAnimating();
}

AnimatedModel::~AnimatedModel() {}

void AnimatedModel::render()
{
    //this->prepareRender();
    pMaterial->bindTextures();

    ShaderProgram* currentShader = ShaderProgram::getCurrentlyBoundShader();
    for(size_t i = 0; i < pSkeleton->getBoneMatrices().size(); i++)
    {
        currentShader->loadUniform("gBones[" + std::to_string(i) + "]", pSkeleton->getBoneMatrices()[i]);
    }


	/*glUniformMatrix4fv(glGetUniformLocation(this->getShader()->getProgramID(),"gBones"), //We find the location of the gBones uniform.
                ,
                GL_FALSE,    //We don't need to transpose the matrices.
           	    &(*)[0][0][0]);*/


    //Load TextureObject related uniforms
    if(pSkeleton->getBoneMatrices().size() > 0)
    {
        //currentShader->loadUniform("isSkeletal", (int)true);
    }

    renderMesh();

    currentShader->loadUniform("isSkeletal", (int)false);
	pMaterial->unbindTextures();
    //this->finishRender();


	pSkeleton->Update();
}


void AnimatedModel::PlaySkeletalAnimation(SkeletalAnimation* anim, bool loop, bool reset_to_start)
{
    pSkeleton->PlaySkeletalAnimation(anim,loop,reset_to_start);
}

void AnimatedModel::StopAnimating()
{
    pSkeleton->StopAnimating();
}