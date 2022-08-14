#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include "bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h"
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "DebugDrawer.h"

struct Instance;


class GumPhysics
{

private:
	enum ConstraintType { _6DOF };

	//btDiscreteDynamicsWorld *pDynamicWorld;
	btMultiBodyDynamicsWorld *pDynamicWorld;
	DebugDrawer* pDebugDrawer;
	Instance* pOnHoverObjectInstance;

	vec3 vWindDir;
	vec3 vRayHitPosition;

public:
	GumPhysics();

	void update();
	void addContraint(btRigidBody *objA, btRigidBody *objB, ConstraintType type);
	void addDebugDrawer(ShaderProgram *shader);
	void drawDebug();
	void addWall(vec3 pos, vec3 size);

	//Getter
	DebugDrawer *getDebugDrawer();
	btMultiBodyDynamicsWorld *getWorld();
	Instance* getMouseOnObjectInstance();
	vec3 getWindDir();

	//Setter
	void setWindDir(vec3 windDir);
	void setScreenSize(vec2 size);
};