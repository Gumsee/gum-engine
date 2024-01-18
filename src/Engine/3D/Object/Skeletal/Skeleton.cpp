#include "Skeleton.h"
#include <Essentials/Tools.h>
#include <Essentials/FPS.h>
#include <System/Output.h>

Skeleton::Skeleton(Bone *rootbone)
    : pRootBone(rootbone),
      fTime(0), fStartTime(0), fEndTime(0),
      pCurrentSkeletalAnimation(nullptr),
      bIsAnimationPlaying(false),
      bShouldAnimationLoop(false)
{
    vBoneMats.resize(100);

    if(pRootBone == nullptr)
        Gum::Output::error("Skeleton: Rootbone is nullptr");
}

void Skeleton::recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform)
{    
    currentBone->updateKeyframeTransform(fTime);

    mat4 globalTransform = parentTransform * currentBone->getTransform();
    if(currentBone->getID() >= 0)
        vBoneMats[currentBone->getID()] = globalTransform * currentBone->getOffsetMatrix();

    //Recursive
    for(unsigned int i = 0; i < currentBone->numChildren(); i++)
        recursiveUpdateBoneMatsVector(currentBone->getChild(i), globalTransform);
}

void Skeleton::update()
{
    if(pCurrentSkeletalAnimation == nullptr)
        return;

    if(!bIsAnimationPlaying)
        return;

    //If we're not playing an SkeletalAnimation, then just give up, do nothing.
    //Update the time variable by adding the delta time of the last frame
    //It's * 0.001f because the delta time is in milliseconds, and we 
    //need it in seconds.
    fTime += FPS::get() * pCurrentSkeletalAnimation->fSpeed;

    //Make sure the time can't be less than our SkeletalAnimation's start time.
    if(fTime < fStartTime)
        fTime = fStartTime;

    //Make sure the time can't be greater than our SkeletalAnimation's end time.
    if(fTime > fEndTime)
    {
        if(bShouldAnimationLoop)
            fTime = fStartTime;
        else
            stopAnimation();
    }

    //Make sure there's nothning left in the vector.
    //for(int i = 0; i < 100; i++)
    //    vBoneMats[i] = mat4();
    
    recursiveUpdateBoneMatsVector(pRootBone, mat4());
}

void Skeleton::playAnimation(SkeletalAnimation* anim, bool loop)
{
    if(anim == nullptr)
        return;
    

    //And this SkeletalAnimation is more important than the current one
    if(pCurrentSkeletalAnimation != nullptr && pCurrentSkeletalAnimation->iPriority > anim->iPriority)
        return;


    pCurrentSkeletalAnimation = anim;
    fStartTime = pCurrentSkeletalAnimation->fStartTime;
    fEndTime = pCurrentSkeletalAnimation->fEndTime;

    bIsAnimationPlaying = true;
    bShouldAnimationLoop = loop;

    recursiveUpdateBoneMatsVector(pRootBone, mat4());
}

void Skeleton::stopAnimation()
{
    fTime = fEndTime;
    pCurrentSkeletalAnimation = nullptr;
    bIsAnimationPlaying = false;
}


//
// Getter
//
std::vector<mat4> Skeleton::getBoneMatrices() { return vBoneMats; }