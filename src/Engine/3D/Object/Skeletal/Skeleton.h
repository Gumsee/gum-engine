#pragma once
#include <vector>
#include <gum-primitives.h>

class Skeleton
{
private:   
    Bone* pRootBone;
    std::vector<SkeletalAnimation*> vAppliedAnimation;
    std::vector<mat4> vBoneMats;
    
    float fTime;
    float fStartTime;
    float fEndTime;

    bool bIsAnimationPlaying;
    bool bShouldAnimationLoop;

    void recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform);

public:
    Skeleton(Bone *rootbone, unsigned int numbones);

    void update(); 
    void addAnimation(SkeletalAnimation* anim);

    std::vector<mat4> getBoneMatrices();
    SkeletalAnimation* getAnimation(const unsigned int& index);
};