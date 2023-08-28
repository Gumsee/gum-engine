#include "Camera.h"
#include <Essentials/Settings.h>
#include <Maths/MatrixFunctions.h>

Camera::Camera(const ivec2& resolution, const Type& type)
{
    iType = type;
    v3ViewDirection = vec3(0.0f, 0.0f, 1.0f);
    v3Up = vec3(0.0f, 1.0f, 0.0f);

    updateProjection(resolution);
    updateView();

    if(pActiveCamera == nullptr)
        pActiveCamera = this;
}

Camera::~Camera()
{
}

void Camera::updateView()
{
    mViewMatrix = Gum::Maths::view(v3ActualPosition, v3ActualPosition + v3ViewDirection, v3Up);
    //v3ViewDirection = vec3(-viewMat[0][2], -viewMat[1][2], -viewMat[1][2]);
}

void Camera::makeActive()
{
    pActiveCamera = this;
}

void Camera::moveForward(const float& f)  { v3ActualPosition += v3ViewDirection * f; }
void Camera::moveBackward(const float& f) { v3ActualPosition += v3ViewDirection * -f; }
void Camera::moveLeft(const float& f) 	  { v3ActualPosition += v3StrafeDirection * -f; }
void Camera::moveRight(const float& f) 	  { v3ActualPosition += v3StrafeDirection * f; }
void Camera::moveUp(const float& f) 	  { v3ActualPosition += v3Up * f; }
void Camera::moveDown(const float& f) 	  { v3ActualPosition += v3Up * -f; }


void Camera::invertPitch()
{
    v3ViewDirection.y *= -1;
    updateView();
}


//
// Getter
//
mat4 Camera::getViewMatrix() const			    { return this->mViewMatrix; }
mat4 Camera::getProjectionMatrix() const		{ return this->mActiveProjectionMatrix; }
mat4 Camera::getOrtho() const				    { return this->mOrthographicMatrix; }
mat4 Camera::getPerspective() const             { return this->mPerspectiveMatrix; }
vec3 Camera::getViewDirection() const			{ return this->v3ViewDirection; }
vec3 Camera::getPosition() const			    { return this->v3ActualPosition; }
float Camera::getFOV() const                    { return this->fFOV; }
float Camera::getZoom() const                   { return this->fZoomfactor; }
Camera::Type Camera::getType() const            { return this->iType; }
Camera* Camera::getActiveCamera()               { return pActiveCamera; }


//
// Setter
//
void Camera::setZoomFactor(const float& factor) { fZoomfactor = factor; updateProjection(v2CurrentResolution); }
void Camera::setZoomSpeed(const float& speed)   { fZoomSpeed = speed; updateProjection(v2CurrentResolution); }
void Camera::setFOV(const float& fov)           { fFOV = fov; updateProjection(v2CurrentResolution); }
void Camera::overrideViewMatrix(mat4 matrix)    { this->mViewMatrix = matrix; }