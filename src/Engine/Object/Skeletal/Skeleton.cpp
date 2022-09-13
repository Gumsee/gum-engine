#include "Skeleton.h"
#include <Essentials/Tools.h>
#include <Essentials/FPS.h>
#include <System/IO/Output.h>

Skeleton::Skeleton(Bone *rootBone, mat4 in_globalInverseTransform)
{
    time = start_time = end_time = 0;
    active_SkeletalAnimation = nullptr;
    anim_play = false;
    anim_loop = false;
    boneMats.resize(100);

    if(rootBone == nullptr)
        Gum::Output::error("Skeleton: Rootbone is nullptr");

    this->rootBone = rootBone;
    globalInverseTransform = in_globalInverseTransform;
    time = start_time = end_time = 0;
    active_SkeletalAnimation = nullptr;
    idle_SkeletalAnimation = nullptr;

    anim_loop = false;
    anim_play = false;
}

void Skeleton::recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform)
{
    if(active_SkeletalAnimation == nullptr)
        return;
    
    currentBone->UpdateKeyframeTransform(time);
    mat4 globalTransform = currentBone->getTransform() * parentTransform;
    mat4 finalMatrix =  currentBone->getOffsetMatrix() * globalTransform * globalInverseTransform; //Mat4 multiplication is broken (probably)
    boneMats[currentBone->getID()] = finalMatrix;

    for(unsigned int i = 0; i < currentBone->numChildren(); i++)
    {
        Bone *childBone = currentBone->getChild(i);
        recursiveUpdateBoneMatsVector(childBone, globalTransform);
    }
}

void Skeleton::Update()
{

    if(!anim_play)
    {
        return;
    }

    //If we're not playing an SkeletalAnimation, then just give up, do nothing.
    //Update the time variable by adding the delta time of the last frame
    //It's * 0.001f because the delta time is in milliseconds, and we 
    //need it in seconds.
    time += FPS::get() * active_SkeletalAnimation->speed;

    //Make sure the time can't be less than our SkeletalAnimation's start time.
    if(time < start_time)
    {
        time = start_time;
    }

    //Make sure the time can't be greater than our SkeletalAnimation's end time.
    if(time > end_time)
    {
        if(anim_loop) //If looping is set, then loop!
        {
            time = start_time;
        }
        else //Else, give up.
        {
            StopAnimating();
        }
    }

    //Make sure there's nothning left in the vector.
    for(int i = 0; i < 100; i++)
    {
        boneMats[i] = mat4();
    }
    recursiveUpdateBoneMatsVector(rootBone, mat4());
}

void Skeleton::PlaySkeletalAnimation(SkeletalAnimation* anim, bool loop, bool reset_to_start)
{
    //If there's an SkeletalAnimation currently playing
    if(active_SkeletalAnimation != nullptr)
    {
        //And this SkeletalAnimation is more important than the current one
        if(anim->priority < active_SkeletalAnimation->priority) //Set the current SkeletalAnimation to the one passed in.
        {
            active_SkeletalAnimation = anim;
        }
        else //Do nothing.
        {
            return;
        }
    }
    else
        //Set the current SkeletalAnimation to the one passed in.
        active_SkeletalAnimation = anim;

    start_time = active_SkeletalAnimation->start_time;
    end_time = active_SkeletalAnimation->end_time;

    anim_play = true;
    anim_loop = loop;
    
    //The reset_to_start variable determines whether or not the SkeletalAnimation
    //should restart upon playing.
    if(reset_to_start)
    {
        time = active_SkeletalAnimation->start_time;
    }
    recursiveUpdateBoneMatsVector(rootBone, rootBone->getTransform());
}

//This function stops animating
void Skeleton::StopAnimating()
{
    time = end_time;
    active_SkeletalAnimation = nullptr;
    anim_play = false;
}
         
//This function sets the idle SkeletalAnimation
//(An idle SkeletalAnimation, for those who don't know, is an SkeletalAnimation
//that plays when no other SkeletalAnimations are playing)
void Skeleton::SetIdleSkeletalAnimation(SkeletalAnimation* in_anim)
{
    idle_SkeletalAnimation = in_anim;
}

std::vector<mat4> Skeleton::getBoneMatrices()
{
    return boneMats;
}