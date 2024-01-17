#pragma once
#include "Skeleton.h"
#include "../SceneObject.h"
#include <Graphics/Object3D.h>
#include <Graphics/Material.h>

class AnimatedModel : public SceneObject
{
private:
    Skeleton* pSkeleton;

public:
	AnimatedModel(std::string file, std::string name);
	~AnimatedModel();

	void render();

    void PlaySkeletalAnimation(SkeletalAnimation *anim, bool loop = false, bool reset_to_start = false);
    void AddSkeletalAnimation(SkeletalAnimation& in_anim);
    void StopAnimating();

    // Getter
    Skeleton* getSkeleton();
};