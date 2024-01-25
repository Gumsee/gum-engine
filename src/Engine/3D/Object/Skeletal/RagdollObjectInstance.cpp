#include "RagdollObjectInstance.h"
#include <Graphics/Object3DInstance.h>
#include <System/Output.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
#include <LinearMath/btVector3.h>
#include "../../World3D.h"
#include "Essentials/Tools.h"
#include "Maths/MatrixFunctions.h"
#include "Skeleton.h"

#define SCALE 10

static btRigidBody* localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	rbInfo.m_additionalDamping = true;
	btRigidBody* body = new btRigidBody(rbInfo);

	return body;
}

RagdollObjectInstance::RagdollObjectInstance(AnimatedModel* obj, World3D* world) 
    : Object3DInstance(obj)
{
    this->iType = OBJECT3D_INSTANCE_TYPE_RAGDOLL;
    this->pWorld = world;
    this->pParentObject = obj;
    this->bIsTriggered = false;

    for(auto it : obj->getSkeleton()->getBoneMatricesWithoutOffset())
        createBody(it.first, it.second);

    
    for(auto it : obj->getSkeleton()->getBoneMatricesWithoutOffset())
        createJoints(it.first);
}

RagdollObjectInstance::~RagdollObjectInstance()
{}

void RagdollObjectInstance::createBody(Bone* bone, mat4 transform)
{
    if(bone->getParent() == nullptr)
        return;

    float height = 0.0f;
    vec3 bonepos = Gum::Maths::positionFromMatrix(transform);
    vec3 parentbonepos = Gum::Maths::positionFromMatrix(pParentObject->getSkeleton()->getBoneMatricesWithoutOffset()[bone->getParent()]);
    height = vec3::distance(bonepos, parentbonepos) * 0.2f;
    std::cout << "Bone distance to parent " << bone->getName() << ": " << height << std::endl;
    

    btCapsuleShape* shape = new btCapsuleShape(btScalar(0.1), btScalar(height));

	btTransform bullettransform;
    bullettransform.setFromOpenGLMatrix(&(transform)[0][0]);
    btRigidBody* body = localCreateRigidBody(btScalar(1.0f), bullettransform, shape);
    body->setDamping(0.05f, 0.85f);
    body->setDeactivationTime(0.8f);
    body->setSleepingThresholds(1.6f, 2.5f);
    ((btDiscreteDynamicsWorld*)pWorld->getPhysics()->getWorld())->addRigidBody(body);
    mRigidbodies[bone] = new CollisionObject(body);
}

void RagdollObjectInstance::createJoints(Bone* bone)
{
    if(bone->getParent() == nullptr || !Tools::mapHasKeyNotNull(mRigidbodies, bone->getParent()))
        return;


    float height = 0.0f;
    vec3 bonepos = Gum::Maths::positionFromMatrix(pParentObject->getSkeleton()->getBoneMatricesWithoutOffset()[bone]);
    vec3 parentbonepos = Gum::Maths::positionFromMatrix(pParentObject->getSkeleton()->getBoneMatricesWithoutOffset()[bone->getParent()]);
    height = vec3::distance(bonepos, parentbonepos) * 0.2f;

    btTransform localA, localB;
    bool useLinearReferenceFrameA = true;
    localA.setIdentity(); 
    localB.setIdentity();
    localA.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
    localB.setOrigin(btVector3(btScalar(0.), btScalar(-height), btScalar(0.)));

    btGeneric6DofConstraint* joint6DOF = new btGeneric6DofConstraint(
        *(btRigidBody*)mRigidbodies[bone->getParent()]->getBody(), 
        *(btRigidBody*)mRigidbodies[bone]->getBody(), 
        localA, localB,
        useLinearReferenceFrameA
    );

    joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.3f,-SIMD_EPSILON,-SIMD_PI*0.3f));
    joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.5f,SIMD_EPSILON,SIMD_PI*0.3f));

    //m_joints[JOINT_SPINE_HEAD] = joint6DOF;
    ((btDiscreteDynamicsWorld*)pWorld->getPhysics()->getWorld())->addConstraint(joint6DOF, true);
}

void RagdollObjectInstance::update()
{
    if(!bIsTriggered)
    {

        for(auto it : mRigidbodies)
        {
            mat4 transform = pParentObject->getSkeleton()->getBoneMatricesWithoutOffset()[it.first];
            btRigidBody* body = (btRigidBody*)it.second->getBody();
            btTransform bullettransform;
            bullettransform.setFromOpenGLMatrix(&(transform)[0][0]);
            body->setWorldTransform(bullettransform);
            body->getMotionState()->setWorldTransform(bullettransform);
            //body->activate();
            body->forceActivationState(DISABLE_SIMULATION);
        }
        return;
    }

    for(auto it : mRigidbodies)
    {
        btRigidBody* body = (btRigidBody*)it.second->getBody();
        Bone* bone = it.first;

        btTransform bullettransform = body->getWorldTransform();
        //body->getMotionState()->getWorldTransform(bullettransform);

        mat4 transform;
        bullettransform.getOpenGLMatrix(&(transform)[0][0]);

        pParentObject->getSkeleton()->getBoneMatrices()[bone->getID()] = transform * bone->getOffsetMatrix();
    }
}

void RagdollObjectInstance::onTransformUpdate()
{
    /*btRigidBody* body = ((btRigidBody*)pRigidbody->getBody());
    btTransform transform; 
    body->getMotionState()->getWorldTransform(transform); 
    transform.setFromOpenGLMatrix(&(mTransformation)[0][0]); 
    body->setWorldTransform(transform);
    body->getMotionState()->setWorldTransform(transform); */
}

void RagdollObjectInstance::triggerRagdoll()
{
    for(auto it : mRigidbodies)
    {
        mat4 transform = pParentObject->getSkeleton()->getBoneMatricesWithoutOffset()[it.first];
        btRigidBody* body = (btRigidBody*)it.second->getBody();
        btTransform bullettransform;
        bullettransform.setFromOpenGLMatrix(&(transform)[0][0]);
        body->setWorldTransform(bullettransform);
        body->getMotionState()->setWorldTransform(bullettransform);
        body->activate();
        body->forceActivationState(ACTIVE_TAG);
    }

    pParentObject->getSkeleton()->stopUpdating(true);
    bIsTriggered = true;
}