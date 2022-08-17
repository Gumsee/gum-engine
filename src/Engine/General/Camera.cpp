#include "Camera.h"
#include "CameraObject.h"
#include <Essentials/Tools.h>
#include <Essentials/Output.h>
#include <Essentials/Input/Mouse.h>
#include <Essentials/Input/Controls.h>
#include <Essentials/Settings.h>
#include <Essentials/Window.h>
#include <Essentials/FPS.h>
#include <Maths/MatrixFunctions.h>
#include "../Object/Object.h"
#include "../General/World.h"

Camera* Camera::ActiveCamera = nullptr;

Camera::Camera(const ivec2& resolution, World* world, Gum::Window* context)
{
    Gum::Output::log("Initializing Camera!");
    this->pContextWindow = context;
    OffsetToPos = new SmoothFloat(10, 5);
    v3ViewDirection = vec3(0.0f, 0.0f, 1.0f);
    UP = vec3(0.0f, 1.0f, 0.0f);
    ROTATIONAL_SPEED = 0.05f;

    OffsetToPos->setMin(3);
    OffsetToPos->setMax(100);

    v2CurrentResolution = resolution;
    iProjectionMode = ProjectionModes::PERSPECTIVE;
    updateProjection(resolution);
    updateView();

    if(world != nullptr)
    {
        if(!world->getObjectManager()->hasObject("GumEngineCamera"))
        {
            CameraMesh camMesh;
            Object *obj = new Object(camMesh.CameraObjectMesh, "GumEngineCamera");
            world->getObjectManager()->addObject(obj);
            obj->getProperties()->pMesh = nullptr;
            pCameraObjectInstance = world->getObjectManager()->getObject("GumEngineCamera")->getInstance();
        }
        else
        {
            pCameraObjectInstance = new Instance();
            world->getObjectManager()->getObject("GumEngineCamera")->addInstance(pCameraObjectInstance);
        }
        pCameraObjectInstance->setPosition(v3Position);
    }
}

Camera::~Camera()
{
    Gum::_delete(OffsetToPos);
}

void Camera::updateProjection(const ivec2& resolution)
{
    if(iProjectionMode == ProjectionModes::PERSPECTIVE)
    {
        projectionMatrix = Gum::Maths::perspective(FOV, (float)resolution.x / (float)resolution.y, 0.1f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
    }
    else
    {
        float halfheight = pContextWindow->getSize().y * 0.25f;
        float halfwidth = pContextWindow->getAspectRatioWidthToHeight() * halfheight;
        projectionMatrix = Gum::Maths::ortho(halfheight, halfwidth, -halfheight, -halfwidth, 0.1f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
    }
    OrthographicMatrix = Gum::Maths::ortho((float)resolution.y, (float)resolution.x, 0.0f, 0.0f, -100.0f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
}

void Camera::update()
{
    if(!isStopped)
    {
        switch(CurrentMode)
        {
            case Modes::CONTROLLED:
            {
                v3ActualPosition = v3Position;
                updateView();
                break;
            }
            case Modes::FIRSTPERSON:
            {
                mouseUpdate();
                v3ActualPosition = v3Position;
                updateView();
                break;
            }
            case Modes::THIRDPERSON:
            {
                OffsetToPos->increaseTarget(pContextWindow->getMouse()->getMouseWheelState() * -1 * ZOOMFACTOR);
                OffsetToPos->update();

                fPitch -= -Gum::Input::Mouse::getDelta().y * 0.1f;
                MouseAngle += -Gum::Input::Mouse::getDelta().x * 0.1f;
                AngleAroundPos = MouseAngle;

                fRoll = fPitch;
                Gum::Maths::clamp(fPitch, -89.99f, 89.99f);

                v3ActualPosition.x = v3Position.x - OffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::sindeg(AngleAroundPos);
                v3ActualPosition.y = v3Position.y + OffsetToPos->get() * Gum::Maths::sindeg(fPitch);
                v3ActualPosition.z = v3Position.z - OffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::cosdeg(AngleAroundPos);

                //viewMat = Math::view(v3ActualPosition, v3Position, UP);
                v3ViewDirection = vec3::normalize(v3Position - v3ActualPosition);
                updateView();

                
                if(iProjectionMode == ProjectionModes::ORTHOGRAPHIC)
                {
                    float halfheight = pContextWindow->getSize().y * (OffsetToPos->get() / OffsetToPos->getMax()) * 0.025f;
                    float halfwidth = pContextWindow->getAspectRatioWidthToHeight() * halfheight;
                    projectionMatrix = Gum::Maths::ortho(halfheight, halfwidth, -halfheight, -halfwidth, -100.0f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
                }

                strafeDirection = vec3::cross(v3ViewDirection, UP);
                rotator = Gum::Maths::rotateMatrix(UP * AngleAroundPos) * Gum::Maths::rotateMatrix(strafeDirection * fRoll);
                break;
            }
            case Modes::FREECAM:
            {
                MOVEMENT_SPEED += pContextWindow->getMouse()->getMouseWheelState();
                if (MOVEMENT_SPEED < 0) { MOVEMENT_SPEED = 0; } //Min Speed is 0

                mouseUpdate();
                if (Gum::Input::Controls::checkControl("Forward", pContextWindow->getKeyboard()))	{ moveForward(); }
                if (Gum::Input::Controls::checkControl("Backward", pContextWindow->getKeyboard()))	{ moveBackward(); }
                if (Gum::Input::Controls::checkControl("Left", pContextWindow->getKeyboard()))  	{ strafeLeft(); }
                if (Gum::Input::Controls::checkControl("Right", pContextWindow->getKeyboard())) 	{ strafeRight(); }
                if (Gum::Input::Controls::checkControl("Down", pContextWindow->getKeyboard()))  	{ moveDown(); }
                if (Gum::Input::Controls::checkControl("Up", pContextWindow->getKeyboard()))	 	{ moveUp();	}
                updateView();
                break;
            }
        }
    }
}

void Camera::updateView()
{
    viewMat = Gum::Maths::view(v3ActualPosition, v3ActualPosition + v3ViewDirection, UP);
    if(pCameraObjectInstance != nullptr)
        pCameraObjectInstance->setPosition(v3Position);
    //v3ViewDirection = vec3(-viewMat[0][2], -viewMat[1][2], -viewMat[1][2]);
}

void Camera::mouseUpdate()
{
    strafeDirection = vec3::cross(v3ViewDirection, UP);
    if(Gum::Input::Mouse::getDelta() != vec2())
    {
        rotator = Gum::Maths::rotateMatrix(UP * -Gum::Input::Mouse::getDelta().x * ROTATIONAL_SPEED) *
                  Gum::Maths::rotateMatrix(strafeDirection * -Gum::Input::Mouse::getDelta().y * ROTATIONAL_SPEED);

        v3ViewDirection = rotator * v3ViewDirection;
        //vec3::clamp(v3ViewDirection, 0.0f, 1.0f);
    }
}

void Camera::stop()  					        { isStopped = true; }
void Camera::start() 					        { isStopped = false; }
void Camera::moveForward() 				        { v3ActualPosition += v3ViewDirection * MOVEMENT_SPEED * FPS::get(); }
void Camera::moveBackward() 			        { v3ActualPosition += v3ViewDirection * -MOVEMENT_SPEED * FPS::get(); }
void Camera::strafeLeft() 				        { v3ActualPosition += strafeDirection * -MOVEMENT_SPEED * FPS::get(); }
void Camera::strafeRight() 				        { v3ActualPosition += strafeDirection* MOVEMENT_SPEED * FPS::get(); }
void Camera::moveUp() 					        { v3ActualPosition += UP * MOVEMENT_SPEED * FPS::get(); }
void Camera::moveDown() 				        { v3ActualPosition += UP * -MOVEMENT_SPEED * FPS::get(); }
void Camera::IncreaseSpeed(const float& speed)  { MOVEMENT_SPEED += speed; }
void Camera::setProjectionMode(const unsigned int& mode) { iProjectionMode = mode; updateProjection(v2CurrentResolution); }


void Camera::invertPitch()
{
    v3ViewDirection.y *= -1;
    updateView();
}


//
// Getter
//
vec3 Camera::getRotation() const					{ return vec3(0); }
mat4 Camera::getViewMatrix() const			        { return this->viewMat; }
mat4 Camera::getProjectionMatrix() const			{ return this->projectionMatrix; }
mat4 Camera::getOrtho() const					    { return this->OrthographicMatrix; }
vec3 Camera::getViewDirection() const				{ return this->v3ViewDirection; }
vec3 Camera::getPosition() const				    { return this->v3ActualPosition; }
float Camera::getSpeed() const						{ return this->MOVEMENT_SPEED; }
int Camera::getMode() const							{ return this->CurrentMode; }
unsigned int Camera::getProjectionMode() const      { return this->iProjectionMode; }


//
// Setter
//
void Camera::setSpeed(const float& speed) 			{ MOVEMENT_SPEED = speed; }
void Camera::setPosition(const vec3& pos) 		    { v3Position = pos; updateView(); }
void Camera::setRotation(const vec3& rot)           { /*v3ViewDirection =  updateView();*/ }
void Camera::setOffset(const float& offset) 		{ OffsetToPos->setTarget(offset); }
void Camera::lookAt(const vec3& lookat) 			{ viewMat = Gum::Maths::view(v3ActualPosition, lookat, UP); v3ViewDirection = vec3(-viewMat[0][2], -viewMat[1][2], -viewMat[1][2]); }
void Camera::setZoomFactor(const int& factor) 		{ ZOOMFACTOR = factor; }
void Camera::setMode(const int& Mode) 				{ CurrentMode = Mode; }