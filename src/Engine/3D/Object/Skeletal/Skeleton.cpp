#include "Skeleton.h"
#include "Primitives/SkeletalAnimation.h"
#include <Essentials/Tools.h>
#include <Essentials/Time.h>
#include <System/Output.h>

Skeleton::Skeleton(Bone *rootbone, unsigned int numbones)
    : pRootBone(rootbone),
      bIsStopUpdating(false)
{
    vBoneMats.resize(numbones);

    if(pRootBone == nullptr)
        Gum::Output::error("Skeleton: Rootbone is nullptr");

    recursiveUpdateBoneMatsVector(pRootBone, mat4());
}

void Skeleton::recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform)
{
    mat4 globalTransform = parentTransform * currentBone->getTransform();
    if(currentBone->getID() >= 0)
    {
        mBoneMatsWithoutOffset[currentBone] = globalTransform;
        vBoneMats[currentBone->getID()] = globalTransform * currentBone->getOffsetMatrix();
    }

    //Recursive
    for(unsigned int i = 0; i < currentBone->numChildren(); i++)
        recursiveUpdateBoneMatsVector(currentBone->getChild(i), globalTransform);
}

void Skeleton::update()
{
    if(bIsStopUpdating || vAppliedAnimation.empty())
        return;

    //Make sure there's nothning left in the vector.
    //for(int i = 0; i < 100; i++)
    //    vBoneMats[i] = mat4();

    for(SkeletalAnimation* animation : vAppliedAnimation)
        animation->applyToBones();

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
std::vector<mat4>& Skeleton::getBoneMatrices()                            { return vBoneMats; }
std::unordered_map<Bone*, mat4>& Skeleton::getBoneMatricesWithoutOffset() { return mBoneMatsWithoutOffset; }
SkeletalAnimation* Skeleton::getAnimation(const unsigned int& index)      { return vAppliedAnimation[index]; }
Bone* Skeleton::getRootBone()                                             { return pRootBone; }


void Skeleton::stopUpdating(bool stopupdating) { this->bIsStopUpdating = stopupdating; }