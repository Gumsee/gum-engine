#pragma once

//STL
#include <vector>

//Assimp
#include <assimp/scene.h>

//Own
#include "SkeletalAnimation.h"
#include "Bone.h"

class Skeleton
{
private:   
    Bone* rootBone;
    mat4 globalInverseTransform;
    std::vector<mat4> boneMats;
    
    float time;

    float start_time;
    float end_time;

    SkeletalAnimation* active_SkeletalAnimation;
    SkeletalAnimation* idle_SkeletalAnimation;

    bool anim_play;
    bool anim_loop;

public:
    Skeleton(Bone *rootBone, mat4 in_globalInverseTransform);

    void recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform);
    void Update(); 
    void PlaySkeletalAnimation(SkeletalAnimation* anim, bool loop, bool reset_to_start);
    void StopAnimating();    
    void SetIdleSkeletalAnimation(SkeletalAnimation* in_anim);

    std::vector<mat4> getBoneMatrices();
};