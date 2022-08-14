#include "AnimatedModel.h"
#include <Essentials/Tools.h>
#include "../../Managers/ObjectManager.h"
#include <OpenGL/ShaderProgram.h>


AnimatedModel::AnimatedModel(std::string file, std::string name)
{
	this->pShader = nullptr;
	pProperties->Name = name;

	//Open object file
	pObjectLoader->open(ObjectManager::MODEL_ASSETS_PATH + file);

	//Combine all meshes into one
	pProperties->pMesh = new Mesh();
	for(size_t i = 0; i < pObjectLoader->vMeshes.size(); i++)
	{
		pProperties->pMesh->addMesh(pObjectLoader->vMeshes[i]);
	}



	load();

    pVertexArrayObject->addAttribute(pVertexVBO, 8, 3, GL_INT, sizeof(Vertex), offsetof(Vertex, JointIDs));
    pVertexArrayObject->addAttribute(pVertexVBO, 9, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Weights));

    
    for(size_t i = 0; i < pObjectLoader->vBones.size(); i++)
    {
        if(pObjectLoader->vBones[i]->getParent() != nullptr)
            std::cout << pObjectLoader->vBones[i]->getName() << " " << pObjectLoader->vBones[i]->getParent()->getName() << std::endl;
    }


    pSkeleton = new Skeleton(pObjectLoader->rootBone, pObjectLoader->globalInverseTransform);

    SkeletalAnimation* PlaceHolder = new SkeletalAnimation("Null", vec2(1, 1), 0, 0, 0);
	PlaySkeletalAnimation(PlaceHolder,false,true);
    StopAnimating();
}

AnimatedModel::~AnimatedModel() {}

void AnimatedModel::render()
{
    this->prepareRender();
    pMaterial->bindTextures();

    ShaderProgram* currentShader = ShaderProgram::getCurrentlyBoundShader();
    for(size_t i = 0; i < pSkeleton->getBoneMatrices().size(); i++)
    {
        currentShader->LoadUniform("gBones[" + std::to_string(i) + "]", pSkeleton->getBoneMatrices()[i]);
    }


	/*glUniformMatrix4fv(glGetUniformLocation(this->getShader()->getProgramID(),"gBones"), //We find the location of the gBones uniform.
                ,
                GL_FALSE,    //We don't need to transpose the matrices.
           	    &(*)[0][0][0]);*/


    //Load TextureObject related uniforms
	currentShader->LoadUniform("transformationMatrix", *getTransformationMatrix());
    currentShader->LoadUniform("isInstanced", (int)false);
    if(pSkeleton->getBoneMatrices().size() > 0)
    {
        currentShader->LoadUniform("isSkeletal", (int)true);
    }

    renderMesh();

    currentShader->LoadUniform("isSkeletal", (int)false);
	pMaterial->unbindTextures();
    this->finishRender();


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