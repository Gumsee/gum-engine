#include "Camera.h"
#include "Graphics/ShaderProgram.h"
#include <Essentials/Settings.h>
#include <Maths/MatrixFunctions.h>
#include <functional>
#include <Desktop/Window.h>

std::function<void()> Camera::pOnViewUpdate = [](){
    ShaderProgram::loadUniformForAll("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
};

Camera::Camera(const ivec2& resolution, const Type& type)
{
    if(pActiveCamera == nullptr)
        pActiveCamera = this;

    iType = type;
    v3ViewDirection = vec3(0.0f, 0.0f, 1.0f);
    v3WorldUp = vec3(0.0f, 1.0f, 0.0f);
    v3WorldFront = vec3(1.0f, 0.0f, 0.0f);

    updateProjection(resolution);
    updateView();
}

void Camera::updateView()
{
    mViewMatrix = Gum::Maths::view(v3ActualPosition, v3ActualPosition + v3ViewDirection, v3WorldUp);
    pOnViewUpdate();
    //v3ViewDirection = vec3(-viewMat[0][2], -viewMat[1][2], -viewMat[1][2]);
}

void Camera::makeActive()
{
    pActiveCamera = this;
    pOnViewUpdate();
}

void Camera::moveForward(const float& f)  { v3ActualPosition += v3ViewDirection * f; }
void Camera::moveBackward(const float& f) { v3ActualPosition += v3ViewDirection * -f; }
void Camera::moveLeft(const float& f) 	  { v3ActualPosition += v3StrafeDirection * -f; }
void Camera::moveRight(const float& f) 	  { v3ActualPosition += v3StrafeDirection * f; }
void Camera::moveUp(const float& f) 	  { v3ActualPosition += v3WorldUp * f; }
void Camera::moveDown(const float& f) 	  { v3ActualPosition += v3WorldUp * -f; }


void Camera::invertPitch()
{
    v3ViewDirection.y *= -1;
    updateView();
}



vec3 Camera::calcMouseRayDirection()
{
    //screen space to normalized device space
    vec4 screen_space(
      (2 * (Gum::Window::CurrentlyBoundWindow->getMouse()->getPosition().x / v2CurrentResolution.x)) - 1,
      -((2 * (Gum::Window::CurrentlyBoundWindow->getMouse()->getPosition().y / v2CurrentResolution.y)) - 1),
      -1.0f,
      1.0f
    );

    mat4 invertedProjection = mat4::inverse(mActiveProjectionMatrix);
    vec4 eye_space = mat4::transpose(invertedProjection) * screen_space;
    eye_space.z = -1.0f;
    eye_space.w = 0.0f;

    //eye space to world space
    vec4 worldspace = mViewMatrix * eye_space;

    vec3 rayDir = worldspace;
    rayDir = vec3::normalize(rayDir);
    return rayDir;
}


//
// Getter
//
vec3 Camera::getUpDirection() const      { return this->v3WorldUp; }
vec3 Camera::getFrontDirection() const   { return this->v3WorldFront; }
mat4 Camera::getViewMatrix() const			 { return this->mViewMatrix; }
mat4 Camera::getProjectionMatrix() const { return this->mActiveProjectionMatrix; }
mat4 Camera::getOrtho() const				     { return this->mOrthographicMatrix; }
mat4 Camera::getPerspective() const      { return this->mPerspectiveMatrix; }
vec3 Camera::getViewDirection() const		 { return this->v3ViewDirection; }
vec3 Camera::getStrafeDirection() const  { return this->v3StrafeDirection; }
vec3 Camera::getPosition() const			   { return this->v3ActualPosition; }
float Camera::getFOV() const             { return this->fFOV; }
float Camera::getZoom() const            { return this->fZoomfactor; }
Camera::Type Camera::getType() const     { return this->iType; }
Camera* Camera::getActiveCamera()        { return pActiveCamera; }


//
// Setter
//
void Camera::setZoomFactor(const float& factor)        { fZoomfactor = factor; updateProjection(v2CurrentResolution); }
void Camera::setZoomSpeed(const float& speed)          { fZoomSpeed = speed; updateProjection(v2CurrentResolution); }
void Camera::setFOV(const float& fov)                  { fFOV = fov; updateProjection(v2CurrentResolution); }
void Camera::overrideViewMatrix(mat4 matrix)           { this->mViewMatrix = matrix; }
void Camera::setMovementSpeed(const float& speed)      { this->fMovementSpeed = speed; }
void Camera::setWorldUpDirection(const vec3& up)       { this->v3WorldUp = up; v3WorldUpFrontCross = vec3::cross(v3WorldFront, v3WorldUp); v3ViewDirection = v3WorldUpFrontCross; updateView(); }
void Camera::setWorldFrontDirection(const vec3& front) { this->v3WorldFront = front; v3WorldUpFrontCross = vec3::cross(v3WorldFront, v3WorldUp); }

void Camera::onViewUpdate(std::function<void()> callback)
{
    if(callback != nullptr)
        pOnViewUpdate = callback;
}