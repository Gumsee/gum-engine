#include "Skeleton.h"
#include "Primitives/SkeletalAnimation.h"
#include <Essentials/Tools.h>
#include <Essentials/FPS.h>
#include <System/Output.h>

Skeleton::Skeleton(Bone *rootbone, unsigned int numbones)
    : pRootBone(rootbone),
      fTime(0), fStartTime(0), fEndTime(0),
      bIsAnimationPlaying(false),
      bShouldAnimationLoop(false)
{
    vBoneMats.resize(numbones);

    if(pRootBone == nullptr)
        Gum::Output::error("Skeleton: Rootbone is nullptr");
}

void Skeleton::recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform)
{
    mat4 globalTransform = parentTransform * currentBone->getTransform();
    if(currentBone->getID() >= 0)
        vBoneMats[currentBone->getID()] = globalTransform * currentBone->getOffsetMatrix();

    //Recursive
    for(unsigned int i = 0; i < currentBone->numChildren(); i++)
        recursiveUpdateBoneMatsVector(currentBone->getChild(i), globalTransform);
}

void Skeleton::update()
{
    if(vAppliedAnimation.empty())
        return;

    //Make sure there's nothning left in the vector.
    //for(int i = 0; i < 100; i++)
    //    vBoneMats[i] = mat4();

    bool replace = true;
    for(SkeletalAnimation* animation : vAppliedAnimation)
    {
        animation->applyToBones(replace);
        replace = false;
    }

    recursiveUpdateBoneMatsVector(pRootBone, mat4());
}

void Skeleton::addAnimation(SkeletalAnimation* anim)
{
    if(anim == nullptr)
        return;
    
    vAppliedAnimation.push_back(anim);
}

//
// Getter
//
std::vector<mat4> Skeleton::getBoneMatrices()                          { return vBoneMats; }
SkeletalAnimation* Skeleton::getAnimation(const unsigned int& index)   { return vAppliedAnimation[index]; }