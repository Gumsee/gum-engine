#include "CollisionObject.h"

#ifdef GUM_USE_BULLET_PHYSICS
#include <BulletDynamics/Dynamics/btRigidBody.h>
#endif

CollisionObject::CollisionObject(void* body)
    : userptr(nullptr)
{
    setBody(body);
    callback = nullptr;
}

CollisionObject::~CollisionObject()
{

}

void CollisionObject::onCollision(std::function<void()> callback)
{
    this->callback = callback;
}

void CollisionObject::updateOnCollision()
{
    if(callback != nullptr)
        callback();
}

void CollisionObject::setBody(void* body)
{
    if(body == nullptr)
        return;

    this->internalBody = body;

    #ifdef GUM_USE_BULLET_PHYSICS
    ((btRigidBody*)internalBody)->setUserPointer(this);
    #endif
}

void* CollisionObject::getBody()
{
    return internalBody;
}

void CollisionObject::setUserPtr(void* ptr)
{
    this->userptr = ptr;
}

void* CollisionObject::getUserPtr()
{
    return this->userptr;
}