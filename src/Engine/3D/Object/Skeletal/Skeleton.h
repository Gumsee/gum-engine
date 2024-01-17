#pragma once
#include <vector>
#include <gum-primitives.h>

class Skeleton
{
private:   
    Bone* rootBone;
    std::vector<mat4> boneMats;
    
    float time;

    float start_time;
    float end_time;

    SkeletalAnimation* active_SkeletalAnimation;
    SkeletalAnimation* idle_SkeletalAnimation;

    bool anim_play;
    bool anim_loop;

public:
    Skeleton(Bone *rootBone);

    void recursiveUpdateBoneMatsVector(Bone *currentBone, mat4 parentTransform);
    void Update(); 
    void PlaySkeletalAnimation(SkeletalAnimation* anim, bool loop, bool reset_to_start);
    void StopAnimating();    
    void SetIdleSkeletalAnimation(SkeletalAnimation* in_anim);

    std::vector<mat4> getBoneMatrices();
};