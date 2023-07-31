#pragma once
//Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>

//Own
#include "Skeleton.h"
#include <OpenGL/Object3D.h>
#include <OpenGL/Material.h>

class AnimatedModel : public Object3D
{
private:
    Skeleton* pSkeleton;
    Material* pMaterial;

public:
	AnimatedModel(std::string file, std::string name);
	~AnimatedModel();

	void render();

    void PlaySkeletalAnimation(SkeletalAnimation *anim, bool loop = false, bool reset_to_start = false);
    void AddSkeletalAnimation(SkeletalAnimation& in_anim);
    void StopAnimating();
};