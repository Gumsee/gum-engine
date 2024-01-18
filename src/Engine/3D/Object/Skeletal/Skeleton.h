#pragma once
#include <vector>
#include <gum-primitives.h>

class Skeleton
{
private:   
    Bone* pRootBone;
    SkeletalAnimation* pCurrentSkeletalAnimation;
    std::vector<mat4> vBoneMats;
    
    float fTime;
    float fStartTime;
    float fEndTime;

    bool bIsAnimationPlaying;
    bool bShouldAnimationLoop;

    void recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform);

public:
    Skeleton(Bone *rootbone);

    void update(); 
    void playAnimation(SkeletalAnimation* anim, bool loop);
    void stopAnimation();

    std::vector<mat4> getBoneMatrices();
};