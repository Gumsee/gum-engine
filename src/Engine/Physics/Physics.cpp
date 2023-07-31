#include "Physics.h"
#include <bullet/LinearMath/btAlignedAllocator.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
//#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include <stdio.h>
//#include <BulletDynamics/Featherstone/btMultiBodyMLCPConstraintSolver.h>

#include <Desktop/IO/Mouse.h>
#include <Desktop/Window.h>
#include <Essentials/FPS.h>
#include <System/Output.h>
#include <System/MemoryManagement.h>

#include "../Rendering/Camera.h"
#include "../Shaders/ShaderManager.h"
#include "../3D/Object/CollisionObject.h"
#include "DebugDrawer.h"

Physics::Physics()
{
	//pDebugDrawer = nullptr;
	this->vRayHitPosition = vec3(0,0,0);
	this->vWindDir = vec3(0,0,0);
    
	Gum::Output::log("Initializing Physics! \t\t\t!!!IN WORK!!!");
	Gum::Output::debug("Physics: Creating collisionConfiguarion");
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


	//Setting default Debugging Shader
	addDebugDrawer();

	Gum::Output::log("Initialized Physics!");
}

Physics::~Physics()
{
	Gum::_delete<btDefaultCollisionConfiguration>(collisionConfiguration);
	Gum::_delete<btCollisionDispatcher>(dispatcher);
	Gum::_delete<btDbvtBroadphase>(overlappingPairCache);
	Gum::_delete<btGhostPairCallback>(m_ghostPairCallback);
	Gum::_delete<btSequentialImpulseConstraintSolver>(solver);
	Gum::_delete<btDiscreteDynamicsWorld>(pDynamicWorld);
	//Gum::_delete(pDebugDrawer);
}

void Physics::update()
{
	Gum::Output::debug("Updating Physics");
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)pDynamicWorld;
	world->stepSimulation(FPS::get(), 100, 1.0f/240);

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
}



void Physics::addContraint(btRigidBody *objA, btRigidBody *objB, ConstraintType type)
{
}



void Physics::addDebugDrawer()
{
	DebugDrawer* pDebugDrawer = new DebugDrawer();

	//btIDebugDraw *draw = new DebugDrawer();
	((btDiscreteDynamicsWorld*)pDynamicWorld)->setDebugDrawer(pDebugDrawer);
	
	pDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawAabb | btIDebugDraw::DebugDrawModes::DBG_DrawWireframe | btIDebugDraw::DebugDrawModes::DBG_DrawText);
	//drawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
}



void Physics::drawDebug()
{
	if(Settings::getSetting(Settings::Names::SHOWDEBUGINFO)) 
	{
        btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)pDynamicWorld;
		//pDebugDrawer->prepare();
		glLineWidth(1);
		world->debugDrawWorld();
        btCollisionObjectArray objs = world->getCollisionWorld()->getCollisionObjectArray();
        for(int i = 0; i < objs.size(); i++)
        {
            CollisionObject* collObj = (CollisionObject*)objs[i]->getUserPointer();
            collObj->updateOnCollision();
        }

		//pDebugDrawer->finish();
	}
}



void Physics::addWall(vec3 pos, vec3 size)
{
	btBoxShape *ColShape;
	ColShape = new btBoxShape(btVector3(btScalar(size.x), btScalar(size.y), btScalar(size.z)));
	btVector3 localInertia(0, 0, 0);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);

    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)pDynamicWorld;
	world->addRigidBody(new btRigidBody(0, myMotionState, ColShape, localInertia));
}




//
// Getter
//
void* Physics::getWorld()                       { return this->pDynamicWorld; }
//DebugDrawer* Physics::getDebugDrawer()		 	{ return this->pDebugDrawer; }
Instance* Physics::getMouseOnObjectInstance() 	{ return this->pOnHoverObjectInstance; }
vec3 Physics::getWindDir() 					    { return this->vWindDir; }

//
// Setter
//
void Physics::setWindDir(vec3 windDir) 	 	{ this->vWindDir = windDir; }