#pragma once
#include "Skeleton.h"
#include "../SceneObject.h"
#include <Graphics/Object3D.h>
#include <Graphics/Material.h>

class RagdollObjectInstance;

class AnimatedModel : public SceneObject
{
private:
    Skeleton* pSkeleton;

public:
	AnimatedModel(const Gum::File& file, const std::string& name);
	~AnimatedModel();

	void render();
    RagdollObjectInstance* addRagdollInstance(World3D* world);

    // Getter
    Skeleton* getSkeleton();
};