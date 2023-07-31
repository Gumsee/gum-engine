#include "CollisionObject.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

CollisionObject::CollisionObject()
{

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
    callback();
}

void CollisionObject::setBody(void* body)
{
    this->internalBody = body;
    ((btRigidBody*)internalBody)->setUserPointer(this);
}

void* CollisionObject::getBody()
{
    return internalBody;
}