#pragma once
#include <gum-maths.h>

struct Instance;
class btRigidBody;

class Physics
{

private:
	enum ConstraintType { _6DOF };

	void* collisionConfiguration;
	void* dispatcher;
	void* overlappingPairCache;
	void* m_ghostPairCallback;
	void* solver;
	void* pDynamicWorld;
	//btMultiBodyDynamicsWorld *pDynamicWorld;
	//DebugDrawer* pDebugDrawer;
	Instance* pOnHoverObjectInstance;

	vec3 vWindDir;
	vec3 vRayHitPosition;

public:
	Physics();
	~Physics();

	void update();
	void addContraint(btRigidBody *objA, btRigidBody *objB, ConstraintType type);
	void addDebugDrawer();
	void drawDebug();
	void addWall(vec3 pos, vec3 size);

	//Getter
	//DebugDrawer *getDebugDrawer();
	void* getWorld();
	Instance* getMouseOnObjectInstance();
	vec3 getWindDir();

	//Setter
	void setWindDir(vec3 windDir);
	void setScreenSize(vec2 size);
};