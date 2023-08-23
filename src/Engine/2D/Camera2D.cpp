#include "Camera2D.h"
#include <Graphics/Framebuffer.h>
#include <Desktop/Window.h>
#include <Essentials/Settings.h>
#include <Maths/MatrixFunctions.h>
#include "System/MemoryManagement.h"
#include "System/Output.h"
#include "World2D.h"

Camera2D::Camera2D(const ivec2& resolution, World2D* world)
    : Camera(resolution, CAMERA2D)
{
    v3ViewDirection = vec3(0.0f, 0.0f, -1.0f);
    pOffsetToPos = new SmoothFloat(10, 5);
    pOffsetToPos->setMin(3);
    pOffsetToPos->setMax(100);
    
    updateProjection(resolution);
    updateView();

    if(world != nullptr)
    {
        //Billboard
    }
}

Camera2D::~Camera2D()
{
    Gum::_delete(pOffsetToPos);
}

void Camera2D::updateProjection(const ivec2& resolution)
{
    fZoomfactor = pOffsetToPos->get();
    v2CurrentResolution = resolution;

    
    float height = (pOffsetToPos->get() / pOffsetToPos->getMax()) * 20.0f;
    float width = Framebuffer::CurrentlyBoundFramebuffer->getAspectRatioWidthToHeight() * height;

    mOrthographicMatrix = Gum::Maths::ortho((float)height, (float)width, -height, -width, -100.0f, 100.0f);
    //mOrthographicMatrix = Framebuffer::CurrentlyBoundFramebuffer->getScreenMatrix();
    mPerspectiveMatrix = Gum::Maths::perspective(fFOV, (float)resolution.x / (float)resolution.y, 0.1f, (float)Settings::getSetting(Settings::Names::RENDERDISTANCE));

    mActiveProjectionMatrix = mOrthographicMatrix;
}

void Camera2D::update()
{
    pOffsetToPos->increaseTarget(Gum::Window::CurrentlyBoundWindow->getMouse()->getMouseWheelState() * -1 * fZoomSpeed);
    if(pOffsetToPos->update())
        updateProjection(v2CurrentResolution);
    v3ActualPosition = vPosition;
    v3ActualPosition.z = 10;
    updateView();
    return;
}


//
// Getter
//


//
// Setter
//