#include "Object.h"
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>


/*Object::addPhysics
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
void Object::addPhysics(int type, bool isActive, float mass, Instance *instance, vec3 special)
{
	if(type >= 0)
	{
		generateBoundingBox(instance);

		btTransform Transform;
		Transform.setIdentity();
		Transform.setOrigin(btVector3(instance->getPosition().x, instance->getPosition().y, instance->getPosition().z));

		btDefaultMotionState* defaultMotionState = new btDefaultMotionState(Transform);
		btRigidBody *body = nullptr;

		instance->CollisionObjectType = type;
		instance->CollisionObjectMass = mass;
		instance->CollisionObjectActivity = isActive;
		instance->CollisionObjectSpecial = special;
		if (type == BOX_SHAPE)
		{		
			btBoxShape *ColShape;
			if (special == vec3(0,0,0))
			{
				ColShape = new btBoxShape(btVector3(
					btScalar(Gum::Maths::distance(BoundingBox.m_vecMax.x, BoundingBox.m_vecMin.x) / 2), 
					btScalar(Gum::Maths::distance(BoundingBox.m_vecMax.y, BoundingBox.m_vecMin.y) / 2), 
					btScalar(Gum::Maths::distance(BoundingBox.m_vecMax.z, BoundingBox.m_vecMin.z) / 2)));
			}
			else
			{
				ColShape = new btBoxShape(btVector3(btScalar(special.x), btScalar(special.y), btScalar(special.z)));;
			}
			
			btVector3 localInertia(0, 0, 0);
			if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

			body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
		}

		if (type == CAPSULE)
		{
			btCapsuleShape *ColShape;
			if (special == vec3(0,0,0))
			{
				ColShape = new btCapsuleShape(BoundingBox.m_vecMax.z, BoundingBox.m_vecMax.y);
			}
			else
			{
				ColShape = new btCapsuleShape(btScalar(special.x), btScalar(special.y));
			}

			btVector3 localInertia(0, 0, 0);
			if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

			body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
		}

		if (type == SPHERE)
		{
			btSphereShape *ColShape;
			if(special == vec3(0,0,0))
			{
				ColShape = new btSphereShape(std::max(std::max(BoundingBox.m_vecMax.x, BoundingBox.m_vecMax.y), BoundingBox.m_vecMax.z));
			}
			else
			{
				ColShape = new btSphereShape(special.x);
			}

			btVector3 localInertia(0, 0, 0);
			if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

			body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
		}

		if (type == CONE)
		{
			btConeShape *ColShape = new btConeShape(special.x, special.y);;

			btVector3 localInertia(0, 0, 0);
			if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

			body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
		}

		if (type == CYLINDER)
		{
			btCylinderShape *ColShape = new btCylinderShape(btVector3(btScalar(special.x), btScalar(special.y), btScalar(special.z)));;

			btVector3 localInertia(0, 0, 0);
			if (isActive) { ColShape->calculateLocalInertia(mass, localInertia); }

			body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
		}

		if (type == PLANE)
		{
			btCollisionShape *ColShape = new btStaticPlaneShape(btVector3(0,1,0), special.x);

			btVector3 localInertia(0, 0, 0);
			if (isActive)
				ColShape->calculateLocalInertia(mass, localInertia);

			body = new btRigidBody(mass, defaultMotionState, ColShape, localInertia);
		}

		if (type == CONVEX)
		{
			//  Transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));

			btTriangleMesh *trimesh = new btTriangleMesh();
			for (unsigned int i = 0; i < pProperties->pMesh->numIndices(); i+=3)
			{
				int index0 = pProperties->pMesh->getIndex(i);
				int index1 = pProperties->pMesh->getIndex(i + 1);
				int index2 = pProperties->pMesh->getIndex(i + 2);
		
				btVector3 vertex0(pProperties->pMesh->getVertex(index0).position.x, pProperties->pMesh->getVertex(index0).position.y, pProperties->pMesh->getVertex(index0).position.z);
				btVector3 vertex1(pProperties->pMesh->getVertex(index1).position.x, pProperties->pMesh->getVertex(index1).position.y, pProperties->pMesh->getVertex(index1).position.z);
				btVector3 vertex2(pProperties->pMesh->getVertex(index2).position.x, pProperties->pMesh->getVertex(index2).position.y, pProperties->pMesh->getVertex(index2).position.z);
		
				trimesh->addTriangle(vertex0, vertex1, vertex2);
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

		instance->setBody(body);
		GumEngine::Physics->getWorld()->addRigidBody(instance->getBody());
		instance->getBody()->setUserPointer(instance);
		this->setActivation(isActive, instance);
	}
}

bool Object::isCollidingWithRay(vec3 ray, int index)
{
	bool Answer = false;
	float Radius = 5.0f;
	Answer = vec3::distance(this->vInstances[index]->getPosition(), ray) < Radius;
	return Answer;
}

void Object::generateBoundingBox(Instance *inst)
{
	if(BoundingBox.m_vecMin == vec3(0,0,0) && BoundingBox.m_vecMax == vec3(0,0,0))
	{
		if(pProperties->pMesh != nullptr)
		{
			std::vector<float> MinX, MaxX, MinY, MaxY, MinZ, MaxZ;

			for (unsigned int i = 0; i < pProperties->pMesh->numVertices(); i++)
			{
				if (pProperties->pMesh->getVertex(i).position.x < 0)
				{
					MinX.push_back(Gum::Maths::distance(pProperties->pMesh->getVertex(i).position.x * inst->getScale().x, 0.0f));
				}
				else
				{
					MaxX.push_back(Gum::Maths::distance(pProperties->pMesh->getVertex(i).position.x * inst->getScale().x, 0.0f));
				}

				if (pProperties->pMesh->getVertex(i).position.y < 0)
				{
					MinY.push_back(Gum::Maths::distance(pProperties->pMesh->getVertex(i).position.y * inst->getScale().y, 0.0f));
				}
				else
				{
					MaxY.push_back(Gum::Maths::distance(pProperties->pMesh->getVertex(i).position.y * inst->getScale().y, 0.0f));
				}

				if (pProperties->pMesh->getVertex(i).position.z < 0)
				{
					MinZ.push_back(Gum::Maths::distance(pProperties->pMesh->getVertex(i).position.z * inst->getScale().z, 0.0f));
				}
				else
				{
					MaxZ.push_back(Gum::Maths::distance(pProperties->pMesh->getVertex(i).position.z * inst->getScale().z , 0.0f));
				}

			}

			std::vector<float>::iterator result;
			if (MinX.size() != 0)
			{
				result = std::max_element(MinX.begin(), MinX.end());
				BoundingBox.m_vecMin.x = -result[0];
			}

			if (MaxX.size() != 0)
			{
				result = std::max_element(MaxX.begin(), MaxX.end());
				BoundingBox.m_vecMax.x = result[0];
			}

			if (MinY.size() != 0)
			{
				result = std::max_element(MinY.begin(), MinY.end());
				BoundingBox.m_vecMin.y = -result[0];
			}

			if (MaxY.size() != 0)
			{
				result = std::max_element(MaxY.begin(), MaxY.end());
				BoundingBox.m_vecMax.y = result[0];
			}

			if (MinZ.size() != 0)
			{
				result = std::max_element(MinZ.begin(), MinZ.end());
				BoundingBox.m_vecMin.z = -result[0];
			}

			if (MaxZ.size() != 0)
			{
				result = std::max_element(MaxZ.begin(), MaxZ.end());
				BoundingBox.m_vecMax.z = result[0];
			}
		}
		else
		{
			Gum::Output::warn("ObjectPhysics: Object's mesh is nullptr, cannot generate BoundingBox! (" + getName() + ")");
		}
	}
}

void Object::getFurthestAwayPoint(Instance *inst)
{
	if(pProperties->pMesh != nullptr)
	{
		std::vector<float> distances;
		for(unsigned int i = 0; i < pProperties->pMesh->numVertices(); i++)
		{
			distances.push_back(vec3::distance(pProperties->pMesh->getVertex(i).position * inst->getScale(), vec3(0.0f)));
		}
		

		this->pProperties->furthestAwayPoint = std::max_element(distances.begin(), distances.end())[0];
	}
}

void Object::setActivation(bool state, int index) { setActivation(state, getInstance(index)); }
void Object::setActivation(bool state, Instance *inst)
{
	if(inst->getBody() != nullptr)
	{
		if(state)	{ inst->getBody()->activate(); }
		else		{ inst->getBody()->forceActivationState(DISABLE_SIMULATION); }
	}
	else
	{
		Gum::Output::warn("ObjectPhysics: Trying to activate rigidbody, that is a nullptr (" + getName() + ").");
	}
}

void Object::forceActivation(bool state, int index)
{
	if(vInstances[index]->getBody() != nullptr)
	{
		if (state) 	{ vInstances[index]->getBody()->forceActivationState(DISABLE_DEACTIVATION); }
		else 		{ vInstances[index]->getBody()->forceActivationState(DISABLE_SIMULATION); }
	}
	else
	{
		Gum::Output::warn("ObjectPhysics: Trying to force activation on a rigidbody, that is a nullptr (" + getName() + ").");
	}
}