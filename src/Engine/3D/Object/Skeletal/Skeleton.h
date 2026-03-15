#pragma once
#include "Primitives/Vertex.h"
#include <unordered_map>
#include <vector>
#include <gum-primitives.h>

class Skeleton
{
private:   
    Bone* pRootBone;
    std::vector<SkeletalAnimation*> vAppliedAnimation;
    std::vector<mat4> vBoneMats;
    std::unordered_map<Bone*, mat4> mBoneMatsWithoutOffset;

    bool bIsStopUpdating;

    void recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform);

public:
    Skeleton(Bone *rootbone, unsigned int numbones);

    void update();
    void addAnimation(SkeletalAnimation* anim);

    std::vector<mat4>& getBoneMatrices();
    std::unordered_map<Bone*, mat4>& getBoneMatricesWithoutOffset();
    SkeletalAnimation* getAnimation(const unsigned int& index);
    Bone* getRootBone();
    
    void stopUpdating(bool stopupdating);
};