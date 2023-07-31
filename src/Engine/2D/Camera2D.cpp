// #include "Camera.h"
// #include "CameraObject.h"
// #include <Essentials/Tools.h>
// #include <System/Output.h>
// #include <Desktop/IO/Mouse.h>
// #include <Desktop/IO/Controls.h>
// #include <Essentials/Settings.h>
// #include <Desktop/Window.h>
// #include <Essentials/FPS.h>
// #include <Maths/MatrixFunctions.h>
// #include <OpenGL/Object3D.h>
// #include "../General/World2D.h"
// #include "../Object/SceneObject.h"

// Camera::Camera(const ivec2& resolution, World2D* world)
// {
//     Gum::Output::log("Initializing Camera!");
//     OffsetToPos = new SmoothFloat(10, 5);
//     v3ViewDirection = vec3(0.0f, 0.0f, 1.0f);
//     UP = vec3(0.0f, 1.0f, 0.0f);
//     ROTATIONAL_SPEED = 1.0f;

//     OffsetToPos->setMin(3);
//     OffsetToPos->setMax(100);

//     iProjectionMode = ProjectionModes::PERSPECTIVE3D;
//     iCurrentMode = Modes::THIRDPERSON;
//     updateProjection(resolution);
//     updateView();

//     if(world != nullptr)
//     {
//         if(!world->getObjectManager()->hasObject("GumEngineCamera"))
//         {
//             CameraMesh camMesh;
//             SceneObject *obj = new SceneObject(camMesh.CameraObjectMesh, "GumEngineCamera");
//             obj->getMaterial()->setColor(vec4(1,1,1,0.3));
            
//             //obj->pMesh = nullptr;
//             pCameraObjectInstance = obj->addInstance();
//             world->getObjectManager()->addObject(obj);
//         }
//         else
//         {
//             pCameraObjectInstance = world->getObjectManager()->getObject("GumEngineCamera")->addInstance();
//         }
//         pCameraObjectInstance->setPosition(v3Position);
//         pCameraObjectInstance->setScale(vec3(0.1f));
//     }

//     if(ActiveCamera == nullptr)
//         ActiveCamera = this;
// }

// Camera::~Camera()
// {
//     Gum::_delete(OffsetToPos);
// }

// void Camera::updateProjection(const ivec2& resolution)
// {
//     v2CurrentResolution = resolution;
//     float halfheight = Framebuffer::CurrentlyBoundFramebuffer->getSize().y * (OffsetToPos->get() / OffsetToPos->getMax()) * 0.025f;
//     float halfwidth = Framebuffer::CurrentlyBoundFramebuffer->getAspectRatioWidthToHeight() * halfheight;
//     mOrthographicMatrix = Gum::Maths::ortho(halfheight, halfwidth, -halfheight, -halfwidth, 0.1f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));
//     mPerspectiveMatrix = Gum::Maths::perspective(FOV, (float)resolution.x / (float)resolution.y, 0.1f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));

//     if(iProjectionMode == ProjectionModes::PERSPECTIVE3D)
//         mActiveProjectionMatrix = mPerspectiveMatrix;
//     else
//         mActiveProjectionMatrix = mOrthographicMatrix;
// }

// void Camera::update()
// {
//     if(!isStopped)
//     {
//         if(iProjectionMode == ORTHOGRAPHIC2D)
//         {
//             OffsetToPos->increaseTarget(Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * -1 * ZOOMFACTOR);
//             if(OffsetToPos->update())
//                 updateProjection(v2CurrentResolution);
//             v3ActualPosition = v3Position;
//             v3ActualPosition.z = -10;
//             updateView();
//             return;
//         }

//         switch(iCurrentMode)
//         {
//             case CONTROLLED:
//             {
//                 v3ActualPosition = v3Position;
//                 updateView();
//                 break;
//             }
//             case FIRSTPERSON:
//             {
//                 mouseUpdate();
//                 v3ActualPosition = v3Position;
//                 updateView();
//                 break;
//             }
//             case THIRDPERSON:
//             {
//                 OffsetToPos->increaseTarget(Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * -1 * ZOOMFACTOR);
//                 if(OffsetToPos->update() && iProjectionMode == ProjectionModes::ORTHOGRAPHIC3D)
//                     updateProjection(v2CurrentResolution);
                    

//                 Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
//                 fPitch -= -(float)mouse->getDelta().y * ROTATIONAL_SPEED;
//                 MouseAngle += -(float)mouse->getDelta().x * ROTATIONAL_SPEED;
//                 AngleAroundPos = MouseAngle;

//                 fRoll = fPitch;
//                 Gum::Maths::clamp(fPitch, -89.99f, 89.99f);

//                 v3ActualPosition.x = v3Position.x - OffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::sindeg(AngleAroundPos);
//                 v3ActualPosition.y = v3Position.y + OffsetToPos->get() * Gum::Maths::sindeg(fPitch);
//                 v3ActualPosition.z = v3Position.z - OffsetToPos->get() * Gum::Maths::cosdeg(fPitch) * Gum::Maths::cosdeg(AngleAroundPos);

//                 //viewMat = Math::view(v3ActualPosition, v3Position, UP);
//                 v3ViewDirection = vec3::normalize(v3Position - v3ActualPosition);
//                 updateView();

                

//                 strafeDirection = vec3::cross(v3ViewDirection, UP);
//                 rotator = Gum::Maths::rotateMatrix(UP * AngleAroundPos) * Gum::Maths::rotateMatrix(strafeDirection * fRoll);
//                 break;
//             }
//             case Modes::FREECAM:
//             {
//                 MOVEMENT_SPEED += Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState();
//                 if (MOVEMENT_SPEED < 0) { MOVEMENT_SPEED = 0; } //Min Speed is 0

//                 mouseUpdate();
//                 if (Gum::IO::Controls::checkControl("Forward", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	{ moveForward(); }
//                 if (Gum::IO::Controls::checkControl("Backward", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	{ moveBackward(); }
//                 if (Gum::IO::Controls::checkControl("Left", Gum::Window::CurrentlyBoundWindow->getKeyboard()))  	{ strafeLeft(); }
//                 if (Gum::IO::Controls::checkControl("Right", Gum::Window::CurrentlyBoundWindow->getKeyboard())) 	{ strafeRight(); }
//                 if (Gum::IO::Controls::checkControl("Down", Gum::Window::CurrentlyBoundWindow->getKeyboard()))  	{ moveDown(); }
//                 if (Gum::IO::Controls::checkControl("Up", Gum::Window::CurrentlyBoundWindow->getKeyboard()))	 	{ moveUp();	}
//                 updateView();
//                 break;
//             }
//             case STATIC:
//                 break;
//         }
//     }
// }

// void Camera::updateView()
// {
//     viewMat = Gum::Maths::view(v3ActualPosition, v3ActualPosition + v3ViewDirection, UP);
//     if(pCameraObjectInstance != nullptr)
//         pCameraObjectInstance->setPosition(v3Position);
//     //v3ViewDirection = vec3(-viewMat[0][2], -viewMat[1][2], -viewMat[1][2]);
// }

// void Camera::mouseUpdate()
// {
//     strafeDirection = vec3::cross(v3ViewDirection, UP);
//     Gum::IO::Mouse* mouse = Gum::Window::CurrentlyBoundWindow->getMouse();
//     if(mouse->getDelta() != vec2())
//     {
//         rotator = Gum::Maths::rotateMatrix(UP * -(float)mouse->getDelta().x * ROTATIONAL_SPEED) *
//                   Gum::Maths::rotateMatrix(strafeDirection * -(float)mouse->getDelta().y * ROTATIONAL_SPEED);

//         v3ViewDirection = rotator * v3ViewDirection;
//         //vec3::clamp(v3ViewDirection, 0.0f, 1.0f);
//     }
// }

// void Camera::stop()  					        { isStopped = true; }
// void Camera::start() 					        { isStopped = false; }
// void Camera::moveForward() 				        { v3ActualPosition += v3ViewDirection * MOVEMENT_SPEED * FPS::get(); }
// void Camera::moveBackward() 			        { v3ActualPosition += v3ViewDirection * -MOVEMENT_SPEED * FPS::get(); }
// void Camera::strafeLeft() 				        { v3ActualPosition += strafeDirection * -MOVEMENT_SPEED * FPS::get(); }
// void Camera::strafeRight() 				        { v3ActualPosition += strafeDirection* MOVEMENT_SPEED * FPS::get(); }
// void Camera::moveUp() 					        { v3ActualPosition += UP * MOVEMENT_SPEED * FPS::get(); }
// void Camera::moveDown() 				        { v3ActualPosition += UP * -MOVEMENT_SPEED * FPS::get(); }
// void Camera::IncreaseSpeed(const float& speed)  { MOVEMENT_SPEED += speed; }


// void Camera::invertPitch()
// {
//     v3ViewDirection.y *= -1;
//     updateView();
// }


// //
// // Getter
// //
// mat4 Camera::getViewMatrix() const			              { return this->viewMat; }
// mat4 Camera::getProjectionMatrix() const			      { return this->mActiveProjectionMatrix; }
// mat4 Camera::getOrtho() const					          { return this->mOrthographicMatrix; }
// mat4 Camera::getPerspective() const                       { return this->mPerspectiveMatrix; }
// vec3 Camera::getViewDirection() const				      { return this->v3ViewDirection; }
// vec3 Camera::getPosition() const				          { return this->v3ActualPosition; }
// float Camera::getSpeed() const						      { return this->MOVEMENT_SPEED; }
// float Camera::getFOV() const                              { return this->FOV; }
// Camera::Modes Camera::getMode() const					  { return this->iCurrentMode; }
// Camera::ProjectionModes Camera::getProjectionMode() const { return this->iProjectionMode; }


// //
// // Setter
// //
// void Camera::setProjectionMode(const ProjectionModes& mode) { iProjectionMode = mode; updateProjection(v2CurrentResolution); }
// void Camera::setSpeed(const float& speed) 			        { MOVEMENT_SPEED = speed; }
// void Camera::setRotationalSpeed(const float& speed)         { ROTATIONAL_SPEED = speed; }
// void Camera::setOffset(const float& offset) 		        { OffsetToPos->setTarget(offset); OffsetToPos->set(offset); update(); }
// void Camera::lookAt(const vec3& lookat) 			        { viewMat = Gum::Maths::view(v3ActualPosition, lookat, UP); v3ViewDirection = vec3(-viewMat[0][2], -viewMat[1][2], -viewMat[1][2]); }
// void Camera::setZoomFactor(const int& factor) 		        { ZOOMFACTOR = factor; }
// void Camera::setMode(const Camera::Modes& Mode) 	        { iCurrentMode = Mode; }