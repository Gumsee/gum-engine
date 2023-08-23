#include "PhysicsObjectInstance.h"
#include <Graphics/Object3DInstance.h>
#include <System/Output.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btVector3.h>

/**
 * type: Type of Collision Shape
 * isActive: Can the object be moved
 * mass: Mass/Weight of the object in the World
 * special: Different use for every type

  Specials:
  - BOX: X,Y,Z used for the size
  - CAPSULE: X, Y used for the Radius and height in that order
  - SPHERE: X used for the Radius
  - CONE: X, Y used for the Radius and height in that order
  - CYLINDER: X,Y,Z used for the Size
  - PLANE: X used for the Size
  - CONVEX: None
 */
PhysicsObjectInstance::PhysicsObjectInstance(Object3D* obj, Shape shape, float mass, World3D* world, vec3 special) : Object3DInstance(obj)
{
    this->iType = OBJECT3D_INSTANCE_TYPE_PHYSICS;
    
    bool isActive = mass == 0.0f;
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(vPosition.x, vPosition.y, vPosition.z));

    btDefaultMotionState* defaultMotionState = new btDefaultMotionState(Transform);

    //instance->CollisionObjectType = type;
    //instance->CollisionObjectMass = mass;
    //instance->CollisionObjectActivity = isActive;
    //instance->CollisionObjectSpecial = special;

    btRigidBody* body = nullptr;
    if (shape == BOX)
    {		
        btBoxShape *ColShape;
        if (special == vec3(0,0,0))
        {
            ColShape = new btBoxShape(btVector3(bBoundingBox.getDimensions().x / 2, bBoundingBox.getDimensions().y / 2, bBoundingBox.getDimensions().z / 2));
        }
        else
        {
            ColShape = new btBoxShape(btVector3(btScalar(special.x), btScalar(special.y), btScalar(special.z)));;
        }
        
        btVector3 localInertia(0, 0, 0);
        if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

        body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
    }

    if (shape == CAPSULE)
    {
        btCapsuleShape *ColShape;
        if (special == vec3(0,0,0))
        {
            ColShape = new btCapsuleShape(bBoundingBox.getSize().z / 2, bBoundingBox.getSize().y / 2);
        }
        else
        {
            ColShape = new btCapsuleShape(btScalar(special.x), btScalar(special.y));
        }

        btVector3 localInertia(0, 0, 0);
        if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

        body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
    }

    if (shape == SPHERE)
    {
        btSphereShape *ColShape;
        if(special == vec3(0,0,0))
        {
            ColShape = new btSphereShape(std::max(std::max(bBoundingBox.getSize().x, bBoundingBox.getSize().y), bBoundingBox.getSize().z) / 2);
        }
        else
        {
            ColShape = new btSphereShape(special.x);
        }

        btVector3 localInertia(0, 0, 0);
        if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

        body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
    }

    if (shape == CONE)
    {
        btConeShape *ColShape = new btConeShape(special.x, special.y);;

        btVector3 localInertia(0, 0, 0);
        if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

        body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
    }

    if (shape == CYLINDER)
    {
        btCylinderShape *ColShape = new btCylinderShape(btVector3(btScalar(special.x), btScalar(special.y), btScalar(special.z)));;

        btVector3 localInertia(0, 0, 0);
        if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

        body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
    }

    if (shape == PLANE)
    {
        btCollisionShape *ColShape = new btStaticPlaneShape(btVector3(0,1,0), special.x);

        btVector3 localInertia(0, 0, 0);
        if (isActive)
            ColShape->calculateLocalInertia(mass, localInertia);

        body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
    }

    if (shape == CONVEX)
    {
        //  Transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));

        btTriangleMesh *trimesh = new btTriangleMesh();
        btVector3 vertex[3];
        for (unsigned int i = 0; i < pParent->getMesh()->numIndices(); i+=3)
        {
            for(unsigned int j = 0; j < 3; j++)
            {
                int index = pParent->getMesh()->getIndex(i+j);
                Vertex vert = pParent->getMesh()->getVertex(index);
                vertex[j] = btVector3(vert.position.x, vert.position.y, vert.position.z);
            }
            trimesh->addTriangle(vertex[0], vertex[1], vertex[2]);
        }
        btConvexShape *ColShape = new btConvexTriangleMeshShape(trimesh);
        btShapeHull *hull = new btShapeHull(ColShape);
        btScalar margin = ColShape->getMargin();
        hull->buildHull(margin);
        ColShape->setUserPointer(hull);

        btScalar Mass = 0.0f;
        btVector3 localInertia(0, 0, 0);
        body = new btRigidBody(Mass, defaultMotionState, ColShape, localInertia);
    }
    if(body == nullptr)
        return;

    pRigidbody = new CollisionObject();
    
    pRigidbody->setBody(body);
    pRigidbody->onCollision([this]() {
		btTransform transform;
        btRigidBody* body = (btRigidBody*)pRigidbody->getBody();
		body->getMotionState()->getWorldTransform(transform);
        this->qRotation = quat(transform.getRotation().getX(), transform.getRotation().getY(), transform.getRotation().getZ(), transform.getRotation().getW());
        this->vPosition = vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
		//transform.setOrigin(transform.getOrigin() - btVector3(rigidbodyOffset.x, rigidbodyOffset.y, rigidbodyOffset.z));		
		transform.getOpenGLMatrix(&mTransformation[0][0]);
		//transform.setOrigin(transform.getOrigin() + btVector3(rigidbodyOffset.x, rigidbodyOffset.y, rigidbodyOffset.z));
		mTransformation *= Gum::Maths::scaleMatrix(vScale);
        if(pParent != nullptr)
            pParent->applyTransformationMatrix(this);
    });
    
    ((btDiscreteDynamicsWorld*)world->getPhysics()->getWorld())->addRigidBody(body);
    this->setActivation(isActive);
}

PhysicsObjectInstance::~PhysicsObjectInstance()
{

}


void PhysicsObjectInstance::onTransformUpdate()
{
    btRigidBody* body = ((btRigidBody*)pRigidbody->getBody());
    btTransform transform; 
    body->getMotionState()->getWorldTransform(transform); 
    transform.setFromOpenGLMatrix(&(mTransformation)[0][0]); 
    body->setWorldTransform(transform);
    body->getMotionState()->setWorldTransform(transform); 
}

bool PhysicsObjectInstance::isCollidingWithRay(vec3 ray)
{
	bool Answer = false;
	float Radius = 5.0f;
	Answer = vec3::distance(vPosition, ray) < Radius;
	return Answer;
}


void PhysicsObjectInstance::setActivation(bool state)
{
	/*if(inst->getBody() != nullptr)
	{
		if(state)	{ inst->getBody()->activate(); }
		else		{ inst->getBody()->forceActivationState(DISABLE_SIMULATION); }
	}
	else
	{
		Gum::Output::warn("ObjectPhysics: Trying to activate rigidbody, that is a nullptr (" + getName() + ").");
	}*/
}

void PhysicsObjectInstance::forceActivation(bool state)
{
	/*if(vInstances[index]->getBody() != nullptr)
	{
		if (state) 	{ vInstances[index]->getBody()->forceActivationState(DISABLE_DEACTIVATION); }
		else 		{ vInstances[index]->getBody()->forceActivationState(DISABLE_SIMULATION); }
	}
	else
	{
		Gum::Output::warn("ObjectPhysics: Trying to force activation on a rigidbody, that is a nullptr (" + getName() + ").");
	}*/
}