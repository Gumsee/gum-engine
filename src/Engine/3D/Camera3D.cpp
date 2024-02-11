#include "Camera3D.h"
#include <Graphics/Framebuffer.h>
#include <Essentials/Tools.h>
#include <Desktop/IO/Mouse.h>
#include <Desktop/IO/Controls.h>
#include <Essentials/Settings.h>
#include <Desktop/Window.h>
#include <Essentials/FPS.h>
#include <Maths/MatrixFunctions.h>
#include <System/MemoryManagement.h>
#include "World3D.h"

Camera3D::Camera3D(const ivec2& resolution, World3D* world)
    : Camera(resolution, CAMERA3D)
{
	this->fMouseAngle = 0.0f;
	this->fAngleAroundPos = 0.0f;
    pOffsetToPos = new SmoothFloat(10, 5);
    pOffsetToPos->setMin(3);
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
    fZoomfactor = pOffsetToPos->get();
    v2CurrentResolution = resolution;
    float halfheight = Framebuffer::CurrentlyBoundFramebuffer->getSize().y * (pOffsetToPos->get() / pOffsetToPos->getMax()) * 0.025f;
    float halfwidth = Framebuffer::CurrentlyBoundFramebuffer->getAspectRatioWidthToHeight() * halfheight;
    mOrthographicMatrix = Gum::Maths::ortho(halfheight, halfwidth, -halfheight, -halfwidth, NEAR_PLANE, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
    mPerspectiveMatrix = Gum::Maths::perspective(fFOV, (float)resolution.x / (float)resolution.y, NEAR_PLANE, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));

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
        {
            v3ActualPosition = vPosition;
            updateView();
            break;
        }
        case FIRSTPERSON:
        {
            mouseUpdate();
            v3ActualPosition = vPosition;
            updateView();
            break;
        }
        case THIRDPERSON:
        {
            pOffsetToPos->increaseTarget(Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * -1 * fZoomSpeed);
            if(pOffsetToPos->update())
                updateProjection(v2CurrentResolution);
                

            Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
            fPitch -= -(float)mouse->getDelta().y * ROTATIONAL_SPEED;
            fMouseAngle += -(float)mouse->getDelta().x * ROTATIONAL_SPEED;
            fAngleAroundPos = fMouseAngle;


            fRoll = fPitch;
            fPitch = Gum::Maths::clamp(fPitch, -89.99f, 89.99f);

            v3ActualPosition.x = vPosition.x - pOffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::sindeg(fAngleAroundPos);
            v3ActualPosition.y = vPosition.y + pOffsetToPos->get() * Gum::Maths::sindeg(fPitch);
            v3ActualPosition.z = vPosition.z - pOffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::cosdeg(fAngleAroundPos);

            //viewMat = Math::view(v3ActualPosition, v3Position, UP);
            v3ViewDirection = vec3::normalize(vPosition - v3ActualPosition);
            updateView();

            

            v3StrafeDirection = vec3::cross(v3ViewDirection, v3Up);
            mRotator = Gum::Maths::rotateMatrix(v3Up * fAngleAroundPos) * Gum::Maths::rotateMatrix(v3StrafeDirection * fRoll);
            break;
        }

        case Modes::FREECAM:
        {
            fMovementSpeed = Gum::Maths::clamp(
                fMovementSpeed + Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * 0.01f, 0.1f, 1.0f
            );

            mouseUpdate();
            if (Gum::IO::Controls::checkControl("Forward", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	{ moveForward(fMovementSpeed); }
            if (Gum::IO::Controls::checkControl("Backward", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	{ moveBackward(fMovementSpeed); }
            if (Gum::IO::Controls::checkControl("Left", Gum::Window::CurrentlyBoundWindow->getKeyboard()))  	{ moveLeft(fMovementSpeed); }
            if (Gum::IO::Controls::checkControl("Right", Gum::Window::CurrentlyBoundWindow->getKeyboard())) 	{ moveRight(fMovementSpeed); }
            if (Gum::IO::Controls::checkControl("Down", Gum::Window::CurrentlyBoundWindow->getKeyboard()))  	{ moveDown(fMovementSpeed); }
            if (Gum::IO::Controls::checkControl("Up", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	 	{ moveUp(fMovementSpeed);	}
            updateView();
            break;
        }
        case STATIC:
            break;
        }
}

void Camera3D::mouseUpdate()
{
    v3StrafeDirection = vec3::cross(v3ViewDirection, v3Up);
    Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
    if(mouse->getDelta() != ivec2(0,0))
    {
        mRotator = Gum::Maths::rotateMatrix(v3Up * -(float)mouse->getDelta().x * ROTATIONAL_SPEED) *
                  Gum::Maths::rotateMatrix(v3StrafeDirection * -(float)mouse->getDelta().y * ROTATIONAL_SPEED);

        v3ViewDirection = mRotator * v3ViewDirection;
        //vec3::clamp(v3ViewDirection, 0.0f, 1.0f);
    }
}

void Camera3D::lookAt(const vec3& lookat)
{ 
    mViewMatrix = Gum::Maths::view(v3ActualPosition, lookat, v3Up); 
    v3ViewDirection = vec3(-mViewMatrix[0][2], -mViewMatrix[1][2], -mViewMatrix[1][2]);
    pOnViewUpdate();
}


//
// Getter
//
Camera3D::Modes Camera3D::getMode() const					  { return this->iCurrentMode; }
Camera3D::ProjectionModes Camera3D::getProjectionMode() const { return this->iProjectionMode; }


//
// Setter
//
void Camera3D::setProjectionMode(const ProjectionModes& mode) { iProjectionMode = mode; updateProjection(v2CurrentResolution); }
void Camera3D::setOffset(const float& offset) 		        { pOffsetToPos->setTarget(offset); pOffsetToPos->set(offset); update(); }
void Camera3D::setMode(const Camera3D::Modes& Mode) 	        { iCurrentMode = Mode; }