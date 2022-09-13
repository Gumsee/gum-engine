#include "Transformation.h"
#include <System/MemoryManagement.h>
#include <Essentials/Tools.h>

Transformation::Transformation()
{
	Position = vec3(0,0,0);
	rigidbodyOffset = vec3(0,0,0);
	Rotation = quat();
	RotationAroundPoint = vec3(0,0,0);
	Scale = vec3(1,1,1);
	isActive = false;
	pTransformationMatrix = new mat4();
	body = nullptr;
}


Transformation::~Transformation() 
{
	Gum::_delete(pTransformationMatrix);
}

void Transformation::setPosition(const vec3& Pos)
{
	this->Position = Pos;

	//First set body's matrix to created Matrix except rotation and then apply it to the created matrix again..
	//Set Body's rotation
	if(body != nullptr)
	{
		btTransform transform;
		body->getMotionState()->getWorldTransform(transform);
		transform.setOrigin(btVector3(Position.x, Position.y, Position.z));

		body->setWorldTransform(transform);
		body->getMotionState()->setWorldTransform(transform);
	}

	//Apply body matrix to transformationMatrix
	updateMatrix();
}

//
//  For Quaternion rotations
//
void Transformation::setRotation(const quat& Rot)
{
	this->Rotation = Rot;

	//Set Body's rotation
	if(body != nullptr)
	{
		btTransform transform;
		body->getMotionState()->getWorldTransform(transform);	
		transform.setRotation(btQuaternion(Rotation.x, Rotation.y, Rotation.z, Rotation.w));
		body->setWorldTransform(transform);
		body->getMotionState()->setWorldTransform(transform);
	}

	//Apply body matrix to transformationMatrix
	updateMatrix();
}


//
//  For Euler rotations
//
void Transformation::setRotation(const vec3& Rot) { setRotation(quat::toQuaternion(Rot)); }

void Transformation::setScale(const vec3& Scale)
{
	if(this->Scale != Scale)
	{
		this->Scale = Scale;
		updateMatrix();
	}
}

void Transformation::updateMatrix()
{
	if (body != nullptr)
	{
		btTransform transform;
		body->getMotionState()->getWorldTransform(transform);
        this->Rotation = quat(transform.getRotation().getX(), transform.getRotation().getY(), transform.getRotation().getZ(), transform.getRotation().getW());
        this->Position = vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
		transform.setOrigin(transform.getOrigin() - btVector3(rigidbodyOffset.x, rigidbodyOffset.y, rigidbodyOffset.z));		
		transform.getOpenGLMatrix(pTransformationMatrix[0][0]);
		transform.setOrigin(transform.getOrigin() + btVector3(rigidbodyOffset.x, rigidbodyOffset.y, rigidbodyOffset.z));
		*pTransformationMatrix *= Gum::Maths::scaleMatrix(Scale);
	}
	else
	{
		*pTransformationMatrix = Gum::Maths::createTransformationMatrix(Position, Rotation, Scale);
	}
	transformChanged = true;
}

void Transformation::setScale(const float& Scale)
{
	if(this->Scale != vec3(Scale))
	{
		this->Scale = vec3(Scale);
		updateMatrix();
	}
}

void Transformation::increasePosition(const vec3& Pos) 				{ setPosition(getPosition() + Pos); }
void Transformation::increaseRotation(const quat& Rot) 				{ setRotation(getRotation() * Rot); }
void Transformation::increaseRotation(const vec3& Rot) 				{ setRotation(quat::toEuler(getRotation()) + Rot); }
void Transformation::increaseScale(const vec3& Scale) 				{ setScale(getScale() + Scale); }
void Transformation::increaseScale(const float& Scale) 				{ setScale(getScale() + vec3(Scale)); }

void Transformation::setBody(btRigidBody* body) 					{ this->body = body; }
void Transformation::setRigidbodyOffset(const vec3& offset) 		{ this->rigidbodyOffset = offset; }
void Transformation::increaseRigidbodyOffset(const vec3& offset) 	{ this->rigidbodyOffset += offset; }
void Transformation::rotateAroundPoint(const vec3& point) 			{ this->RotationAroundPoint = point; }
void Transformation::setActivation(const bool& state) 				{ this->isActive = state; }
void Transformation::setMatrixPtr(mat4* matrix) 					{ this->pTransformationMatrix = matrix; }
void Transformation::setMatrix(const mat4& transmatrix) 				
{ 
	*this->pTransformationMatrix = transmatrix; 
	if(body != nullptr) 
	{ 
		btTransform transform; 
		body->getMotionState()->getWorldTransform(transform); 
		transform.setFromOpenGLMatrix(&transmatrix[0][0]); 
		body->setWorldTransform(transform);
		body->getMotionState()->setWorldTransform(transform); 
	} 
	transformChanged = true;
} 


vec3 Transformation::getRigidbodyOffset() const			{ return this->rigidbodyOffset; }
vec3 Transformation::getRotationAroundPoint() const		{ return RotationAroundPoint; }
quat Transformation::getRotation() const 				{ return Rotation; }
vec3 Transformation::getPosition() const				{ return Position; }
vec3 Transformation::getScale() const					{ return Scale; }
float Transformation::getScalef() const					{ return Scale.x; }
btRigidBody *Transformation::getBody() 					{ return body; }
mat4 Transformation::getMatrix() const					{ return *pTransformationMatrix; }
mat4 *Transformation::getMatrixPtr() 					{ return pTransformationMatrix; }