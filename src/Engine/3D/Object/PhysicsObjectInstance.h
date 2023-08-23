#pragma once
#include <Graphics/Object3DInstance.h>
#include "CollisionObject.h"
#include "../World3D.h"

#define OBJECT3D_INSTANCE_TYPE_PHYSICS 1

class PhysicsObjectInstance : public Object3DInstance
{
public:
    enum Shape
    {
        BOX,
        CAPSULE,
        SPHERE,
        CONE,
        CYLINDER,
        PLANE,
        CONVEX,
    };

private:
    CollisionObject* pRigidbody = nullptr;

public:
    PhysicsObjectInstance(Object3D* obj, Shape shape, float mass, World3D* world, vec3 special = vec3(0,0,0));
    ~PhysicsObjectInstance();

    void onTransformUpdate();

    bool isCollidingWithRay(vec3 ray);
    void setActivation(bool state);
    void forceActivation(bool state);
};