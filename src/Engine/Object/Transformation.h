#pragma once
#include <LinearMath/btTransform.h>
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <gum-maths.h>

class Transformation
{
private:
	vec3 Position;
	vec3 rigidbodyOffset;
	vec3 RotationAroundPoint;
	vec3 Scale;
	quat Rotation;

	btRigidBody *body;
	mat4* pTransformationMatrix;

	bool isActive;

public:
	Transformation();
	~Transformation();

	bool transformChanged = false;
	void updateMatrix();

	

	//Increaser
	void increasePosition(const vec3& Pos);
	void increaseRotation(const quat& Rot);
	void increaseRotation(const vec3& Rot);
	void increaseScale(const vec3& Scale);
	void increaseScale(const float& Scale);
	void increaseRigidbodyOffset(const vec3& offset);

	//Setter
	void setActivation(const bool& state);
	void setPosition(const vec3& Pos);
	void setRotation(const quat& Rot);
	void setRotation(const vec3& Rot);
	void setScale(const vec3& Scale);
	void setScale(const float& Scale);
	void setBody(btRigidBody* body);
	void setRigidbodyOffset(const vec3& offset);
	void setMatrix(const mat4& transmatrix);
	void rotateAroundPoint(const vec3& point);
	void setMatrixPtr(mat4* matrix);

	//Getter
	vec3 getRigidbodyOffset() const;
	vec3 getRotationAroundPoint() const;
	vec3 getPosition() const;
	quat getRotation() const;
	vec3 getScale() const;
	float getScalef() const;
	btRigidBody *getBody();
	mat4 getMatrix() const;
	mat4 *getMatrixPtr();
};

