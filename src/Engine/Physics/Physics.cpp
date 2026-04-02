#include "Physics.h"


#include <Desktop/IO/Mouse.h>
#include <Desktop/Window.h>
#include <Essentials/Time.h>
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <stdio.h>

#include "../Rendering/Camera.h"
#include "../3D/Object/CollisionObject.h"

#ifdef GUM_USE_BULLET_PHYSICS
#include <LinearMath/btAlignedAllocator.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "DebugDrawer.h"
#endif
#include <Essentials/Settings.h>

Physics::Physics()
{
	//pDebugDrawer = nullptr;
	this->vRayHitPosition = vec3(0,0,0);
	this->vWindDir = vec3(0,0,0);
    
	Gum::Output::log("Initializing Physics! \t\t\t!!!IN WORK!!!");
	Gum::Output::debug("Physics: Creating collisionConfiguarion");
    
    #ifdef GUM_USE_BULLET_PHYSICS
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	//Gum::Output::debug("Physics: Creating collisionDispatcher");
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	//Gum::Output::debug("Physics: Creating BroadphaseInterface");
	btDbvtBroadphase* overlappingPairCache = new btDbvtBroadphase();
	btGhostPairCallback* m_ghostPairCallback = new btGhostPairCallback();
	overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	//Gum::Output::debug("Physics: Creating SequentialImpulseConstraintSolver");
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	//btMultiBodyConstraintSolver* solver = new btMultiBodyConstraintSolver();

	//Gum::Output::debug("Physics: Creating Dynamic world");
	btDiscreteDynamicsWorld* dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicWorld->setGravity(btVector3(0, -9.81, 0));
	dynamicWorld->getSolverInfo().m_globalCfm = 1e-3;
	dynamicWorld->setForceUpdateAllAabbs(false);
    /*pDynamicWorld->setInternalTickCallback([](btDynamicsWorld* world, float timeStep){
        for(int i = 0; i < world->getDispatcher()->getNumManifolds(); i++)
        {
            btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
            CollisionObject* collObj = (CollisionObject*)contactManifold->getBody0()->getUserPointer();
            collObj->updateOnCollision();
        }
    });*/

    this->collisionConfiguration = collisionConfiguration;
    this->dispatcher = dispatcher;
    this->overlappingPairCache = overlappingPairCache;
    this->m_ghostPairCallback = m_ghostPairCallback;
    this->solver = solver;
    this->pDynamicWorld = dynamicWorld;
    #endif


	//Setting default Debugging Shader
	addDebugDrawer();

	Gum::Output::log("Initialized Physics!");
}

Physics::~Physics()
{
    #ifdef GUM_USE_BULLET_PHYSICS
	Gum::_delete<btDefaultCollisionConfiguration>(collisionConfiguration);
	Gum::_delete<btCollisionDispatcher>(dispatcher);
	Gum::_delete<btDbvtBroadphase>(overlappingPairCache);
	Gum::_delete<btGhostPairCallback>(m_ghostPairCallback);
	Gum::_delete<btSequentialImpulseConstraintSolver>(solver);
	Gum::_delete<btDiscreteDynamicsWorld>(pDynamicWorld);
	//Gum::_delete(pDebugDrawer);
    #endif
}

void Physics::update()
{
    #ifdef GUM_USE_BULLET_PHYSICS
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)pDynamicWorld;
	world->stepSimulation(Time::getFrametime(), 100, 1.0f/240);

	vec3 out_origin = Camera::getActiveCamera()->getPosition();
	vec3 out_end = out_origin + Gum::Window::CurrentlyBoundWindow->getMouse()->getRayDirection()*1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(out_origin.x, out_origin.y, out_origin.z), 
		btVector3(out_end.x, out_end.y, out_end.z)
	);
	world->rayTest(
		btVector3(out_origin.x, out_origin.y, out_origin.z), 
		btVector3(out_end.x, out_end.y, out_end.z), 
		RayCallback
	);

	vRayHitPosition = vec3(0);
	if(RayCallback.hasHit()) 
	{
		pOnHoverObjectInstance = (Instance*)RayCallback.m_collisionObject->getUserPointer();
		vRayHitPosition = vec3(RayCallback.m_hitPointWorld.x(),RayCallback.m_hitPointWorld.y(),RayCallback.m_hitPointWorld.z());
		//onHoverObject->setMouseHover(true);

	}
    #endif
}



void Physics::addContraint([[maybe_unused]]btRigidBody *objA, [[maybe_unused]]btRigidBody *objB, [[maybe_unused]]ConstraintType type)
{
}



void Physics::addDebugDrawer()
{
    #ifdef GUM_USE_BULLET_PHYSICS
	DebugDrawer* pDebugDrawer = new DebugDrawer();

	//btIDebugDraw *draw = new DebugDrawer();
	((btDiscreteDynamicsWorld*)pDynamicWorld)->setDebugDrawer(pDebugDrawer);
	
	pDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawAabb | btIDebugDraw::DebugDrawModes::DBG_DrawWireframe | btIDebugDraw::DebugDrawModes::DBG_DrawText);
	//drawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
    #endif
}



void Physics::drawDebug()
{
	if(Settings::getSetting(Settings::Names::SHOWDEBUGINFO)) 
	{
        #ifdef GUM_USE_BULLET_PHYSICS
        btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)pDynamicWorld;
		//pDebugDrawer->prepare();
		world->debugDrawWorld();
        btCollisionObjectArray objs = world->getCollisionWorld()->getCollisionObjectArray();
        for(int i = 0; i < objs.size(); i++)
        {
            CollisionObject* collObj = (CollisionObject*)objs[i]->getUserPointer();
            collObj->updateOnCollision();
        }

		//pDebugDrawer->finish();
        #endif
	}
}



void Physics::addWall([[maybe_unused]]vec3 pos, [[maybe_unused]]vec3 size)
{
    #ifdef GUM_USE_BULLET_PHYSICS
	btBoxShape *ColShape;
	ColShape = new btBoxShape(btVector3(btScalar(size.x), btScalar(size.y), btScalar(size.z)));
	btVector3 localInertia(0, 0, 0);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);

    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)pDynamicWorld;
	world->addRigidBody(new btRigidBody(0, myMotionState, ColShape, localInertia));
    #endif
}




//
// Getter
//
void* Physics::getWorld()                     { return this->pDynamicWorld; }
//DebugDrawer* Physics::getDebugDrawer()		 	{ return this->pDebugDrawer; }
Instance* Physics::getMouseOnObjectInstance() { return this->pOnHoverObjectInstance; }
vec3 Physics::getWindDir() 					          { return this->vWindDir; }

//
// Setter
//
void Physics::setWindDir(vec3 windDir) 	 	{ this->vWindDir = windDir; }