#pragma once
#include <Graphics/Object3DInstance.h>
#include <unordered_map>
#include "AnimatedModel.h"
#include "../CollisionObject.h"

#define OBJECT3D_INSTANCE_TYPE_RAGDOLL 2

class World3D;

class RagdollObjectInstance : public Object3DInstance
{
public:
    enum Shape
    {
        BOX,
        CAPSULE,
    };

private:
    std::unordered_map<Bone*, CollisionObject*> mRigidbodies;
    World3D* pWorld;
    AnimatedModel* pParentObject;
    bool bIsTriggered;

    void recursiveGenerateRagdoll(Bone *currentBone, mat4 parentTransform);
    void createBody(Bone* bone, [[maybe_unused]] mat4 transform);
    void createJoints(Bone* bone);

public:
    RagdollObjectInstance(AnimatedModel* obj, World3D* world);
    ~RagdollObjectInstance();

    void update() override;

    void onTransformUpdate() override;

    void triggerRagdoll();
};