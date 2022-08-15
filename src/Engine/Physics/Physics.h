#pragma once
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
//#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include "DebugDrawer.h"

struct Instance;


class Physics
{

private:
	enum ConstraintType { _6DOF };

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* overlappingPairCache;
	btGhostPairCallback* m_ghostPairCallback;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld *pDynamicWorld;
	//btMultiBodyDynamicsWorld *pDynamicWorld;
	DebugDrawer* pDebugDrawer;
	Instance* pOnHoverObjectInstance;

	vec3 vWindDir;
	vec3 vRayHitPosition;

public:
	Physics();
	~Physics();

	void update();
	void addContraint(btRigidBody *objA, btRigidBody *objB, ConstraintType type);
	void addDebugDrawer(ShaderProgram *shader);
	void drawDebug();
	void addWall(vec3 pos, vec3 size);

	//Getter
	DebugDrawer *getDebugDrawer();
	btDiscreteDynamicsWorld *getWorld();
	Instance* getMouseOnObjectInstance();
	vec3 getWindDir();

	//Setter
	void setWindDir(vec3 windDir);
	void setScreenSize(vec2 size);
};