#include "Camera3D.h"
#include <Graphics/Framebuffer.h>
#include <Essentials/Tools.h>
#include <Desktop/IO/Mouse.h>
#include <Desktop/IO/Controls.h>
#include <Essentials/Settings.h>
#include <Desktop/Window.h>
#include <Essentials/Time.h>
#include <Maths/MatrixFunctions.h>
#include <System/MemoryManagement.h>
#include "World3D.h"

Camera3D::Camera3D(const ivec2& resolution, World3D* world)
    : Camera(resolution, CAMERA3D)
{
  this->fPitch = 30.0f;
  this->fAngleAroundPos = -30.0f;
  pOffsetToPos = new SmoothFloat(10, 5);
  pOffsetToPos->setMin(1);
  pOffsetToPos->setMax(100);

  iProjectionMode = ProjectionModes::PERSPECTIVE;
  iCurrentMode = Modes::THIRDPERSON;
  updateProjection(resolution);
  updateView();

  if(world != nullptr)
  {
    //Add billboard
  }
}

Camera3D::~Camera3D()
{
  Gum::_delete(pOffsetToPos);
}

void Camera3D::updateProjection(const ivec2& resolution)
{
  v2CurrentResolution = resolution;
  float aspect = fAspectRatio;
  if(aspect == 0.0f)
    aspect = (float)resolution.x / (float)resolution.y;
  
  float halfheight = resolution.y * (fZoomfactor / pOffsetToPos->getMax()) * 0.025f;
  float halfwidth = aspect * halfheight;
  mOrthographicMatrix = Gum::Maths::ortho(halfheight, halfwidth, -halfheight, -halfwidth, NEAR_PLANE, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
  mPerspectiveMatrix = Gum::Maths::perspective(fFOV, aspect, NEAR_PLANE, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));

  if(iProjectionMode == ProjectionModes::PERSPECTIVE)
    mActiveProjectionMatrix = mPerspectiveMatrix;
  else
    mActiveProjectionMatrix = mOrthographicMatrix;
}

void Camera3D::update()
{
  switch(iCurrentMode)
  {
    case CONTROLLED:
      v3ActualPosition = vPosition;
      updateView();
      break;
      
    case FIRSTPERSON:
      mouseUpdate();
      v3ActualPosition = vPosition;
      updateView();
      break;
      
    case THIRDPERSON:
      updateZoom();
      //updateProjection(v2CurrentResolution);
      thirdPersonMotionUpdate();
      break;
      
    case THIRDPERSON_WITH_DRAGGING:
      updateZoom();
      if(Gum::IO::Controls::checkControl("CameraRotationMotionToggle", Gum::Window::CurrentlyBoundWindow->getKeyboard()))
        thirdPersonMotionUpdate();
      else if(Gum::IO::Controls::checkControl("CameraDraggingMotionToggle", Gum::Window::CurrentlyBoundWindow->getKeyboard()))
      {
        Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
        vec3 start, end;
        if(Physics::calcRayPlaneIntersection(v3ActualPosition, calcScreenRayDirection(mouse->getLastPosition()), vec3(0,0,0), vec3(0,0,1), start)
        && Physics::calcRayPlaneIntersection(v3ActualPosition, calcScreenRayDirection(mouse->getPosition()),     vec3(0,0,0), vec3(0,0,1), end))
          {
            setPosition(vec3::clamp(vPosition + (start - end), bPositionBoundaries.getPos(), bPositionBoundaries.getSize()));
            v3ActualPosition = vPosition 
            + v3WorldFront * pOffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::sindeg(fAngleAroundPos)
            + v3WorldUp * pOffsetToPos->get() * Gum::Maths::sindeg(fPitch)
            + v3WorldUpFrontCross * pOffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::cosdeg(fAngleAroundPos);
            updateView();
          }
      }
      break;

    case FREECAM:
      fMovementSpeed = Gum::Maths::clamp(
        fMovementSpeed + Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * 0.5f, 0.1f, 100.0f
      );

      mouseUpdate();
      if (Gum::IO::Controls::checkControl("Forward", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	{ moveForward(fMovementSpeed * Time::getFrametime()); }
      if (Gum::IO::Controls::checkControl("Backward", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	{ moveBackward(fMovementSpeed * Time::getFrametime()); }
      if (Gum::IO::Controls::checkControl("Left", Gum::Window::CurrentlyBoundWindow->getKeyboard()))  	{ moveLeft(fMovementSpeed * Time::getFrametime()); }
      if (Gum::IO::Controls::checkControl("Right", Gum::Window::CurrentlyBoundWindow->getKeyboard())) 	{ moveRight(fMovementSpeed * Time::getFrametime()); }
      if (Gum::IO::Controls::checkControl("Down", Gum::Window::CurrentlyBoundWindow->getKeyboard()))  	{ moveDown(fMovementSpeed * Time::getFrametime()); }
      if (Gum::IO::Controls::checkControl("Up", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	 	{ moveUp(fMovementSpeed * Time::getFrametime());	}
      updateView();
      break;
      
    case STATIC:
      break;
  }
}

bool Camera3D::updateZoom()
{
  pOffsetToPos->increaseTarget(Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * -1 * fZoomSpeed);
  bool ret = pOffsetToPos->update();
  fZoomfactor = pOffsetToPos->get();
  return ret;
}

void Camera3D::mouseUpdate()
{
  v3StrafeDirection = vec3::cross(v3ViewDirection, v3WorldUp);
  Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
  if(mouse->getDelta() != ivec2(0,0))
  {
    mRotator = Gum::Maths::rotateMatrix(v3WorldUp * -(float)mouse->getDelta().x * ROTATIONAL_SPEED) *
               Gum::Maths::rotateMatrix(v3StrafeDirection * -(float)mouse->getDelta().y * ROTATIONAL_SPEED);

    v3ViewDirection = mRotator * v3ViewDirection;
    //vec3::clamp(v3ViewDirection, 0.0f, 1.0f);
  }
}

void Camera3D::thirdPersonMotionUpdate()
{
  Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
  fPitch += (float)mouse->getDelta().y * ROTATIONAL_SPEED;
  fAngleAroundPos -= (float)mouse->getDelta().x * ROTATIONAL_SPEED;

  fRoll = fPitch;
  fPitch = Gum::Maths::clamp(fPitch, -89.99f, 89.99f);

  
  v3ActualPosition = vPosition 
  + v3WorldFront * pOffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::sindeg(fAngleAroundPos)
  + v3WorldUp * pOffsetToPos->get() * Gum::Maths::sindeg(fPitch)
  + v3WorldUpFrontCross * pOffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::cosdeg(fAngleAroundPos);

  v3ViewDirection = vec3::normalize(vPosition - v3ActualPosition);
  v3StrafeDirection = vec3::cross(v3ViewDirection, v3WorldUp);
  updateView();
}

void Camera3D::lookAt(const vec3& lookat)
{ 
  mViewMatrix = Gum::Maths::view(v3ActualPosition, lookat, v3WorldUp); 
  v3ViewDirection = vec3(-mViewMatrix[0][2], -mViewMatrix[1][2], -mViewMatrix[1][2]);
  pOnViewUpdate();
}


//
// Getter
//
Camera3D::Modes Camera3D::getMode() const					            { return this->iCurrentMode; }
Camera3D::ProjectionModes Camera3D::getProjectionMode() const { return this->iProjectionMode; }
bbox3& Camera3D::getPositionBoundaries()                      { return this->bPositionBoundaries; }

//
// Setter
//
void Camera3D::setPositionBoundaries(const bbox3& boundaries) { this->bPositionBoundaries = boundaries; }
void Camera3D::setProjectionMode(const ProjectionModes& mode) { iProjectionMode = mode; updateProjection(v2CurrentResolution); }
void Camera3D::setOffset(const float& offset) 		            { pOffsetToPos->setTarget(offset); pOffsetToPos->set(offset); update(); }
void Camera3D::increaseZoom(const float& zoom)                { pOffsetToPos->increaseTarget(zoom); }
void Camera3D::setMode(const Camera3D::Modes& Mode)
{ 
  iCurrentMode = Mode; 
  switch(iCurrentMode)
  {
    case FREECAM:
    case CONTROLLED:
    case FIRSTPERSON:
    case STATIC:
      update();
      break;
      
    case THIRDPERSON:
    case THIRDPERSON_WITH_DRAGGING:
      updateZoom();
      thirdPersonMotionUpdate();
      break;
  }
}