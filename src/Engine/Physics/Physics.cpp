#include "Physics.h"
#include <bullet/LinearMath/btAlignedAllocator.h>
#include <stdio.h>
//#include <BulletDynamics/Featherstone/btMultiBodyMLCPConstraintSolver.h>

#include <Essentials/Input/Mouse.h>
#include <Essentials/FPS.h>
#include <Essentials/Output.h>

#include "../General/Camera.h"
#include "../Managers/ShaderManager.h"

Physics::Physics()
{
	pDebugDrawer = nullptr;
	this->vRayHitPosition = vec3(0,0,0);
	this->vWindDir = vec3(0,0,0);
    
	Gum::Output::log("Initializing Physics! \t\t\t!!!IN WORK!!!");
	Gum::Output::debug("Physics: Creating collisionConfiguarion");
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	//Gum::Output::debug("Physics: Creating collisionDispatcher");
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	//Gum::Output::debug("Physics: Creating BroadphaseInterface");
	overlappingPairCache = new btDbvtBroadphase();
	m_ghostPairCallback = new btGhostPairCallback();
	overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	//Gum::Output::debug("Physics: Creating SequentialImpulseConstraintSolver");
	solver = new btSequentialImpulseConstraintSolver();
	//btMultiBodyConstraintSolver* solver = new btMultiBodyConstraintSolver();

	//Gum::Output::debug("Physics: Creating Dynamic world");
	pDynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	pDynamicWorld->setGravity(btVector3(0, -9.81, 0));
	pDynamicWorld->getSolverInfo().m_globalCfm = 1e-3;
	pDynamicWorld->setForceUpdateAllAabbs(false);    


	//Setting default Debugging Shader
	addDebugDrawer(Gum::ShaderManager::getShaderProgram("PhysicsDebuggingShader"));

	Gum::Output::log("Initialized Physics!");
}

Physics::~Physics()
{
	delete collisionConfiguration;
	delete dispatcher;
	delete overlappingPairCache;
	delete m_ghostPairCallback;
	delete solver;
	delete pDynamicWorld;
}

void Physics::update()
{
	Gum::Output::debug("Updating Physics");
	pDynamicWorld->stepSimulation(FPS::get(), 100, 1.0f/240);

	vec3 out_origin = Camera::ActiveCamera->getPosition();
	vec3 out_end = out_origin + Gum::Window::CurrentlyBoundWindow->getMouse()->getRayDirection()*1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(out_origin.x, out_origin.y, out_origin.z), 
		btVector3(out_end.x, out_end.y, out_end.z)
	);
	pDynamicWorld->rayTest(
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



void Physics::addDebugDrawer(ShaderProgram *shader)
{
	if(pDebugDrawer != nullptr)
		delete pDebugDrawer;
	pDebugDrawer = new DebugDrawer(shader);

	//btIDebugDraw *draw = new DebugDrawer();
	pDynamicWorld->setDebugDrawer(pDebugDrawer);
	
	pDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawAabb | btIDebugDraw::DebugDrawModes::DBG_DrawWireframe | btIDebugDraw::DebugDrawModes::DBG_DrawText);
	//drawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
}



void Physics::drawDebug()
{
	if(Settings::getSetting(Settings::Names::SHOWDEBUGINFO)) 
	{
		pDebugDrawer->prepare();
		glLineWidth(1);
		pDynamicWorld->debugDrawWorld();

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
	pDynamicWorld->addRigidBody(new btRigidBody(0, myMotionState, ColShape, localInertia));
}




//
// Getter
//
btDiscreteDynamicsWorld* Physics::getWorld()  	{ return this->pDynamicWorld; }
DebugDrawer* Physics::getDebugDrawer()		 	{ return this->pDebugDrawer; }
Instance* Physics::getMouseOnObjectInstance() 	{ return this->pOnHoverObjectInstance; }
vec3 Physics::getWindDir() 					    { return this->vWindDir; }

//
// Setter
//
void Physics::setWindDir(vec3 windDir) 	 	{ this->vWindDir = windDir; }