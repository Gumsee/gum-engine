#pragma once
#include <Primitives/Transformable.h>
#include <Essentials/SmoothFloat.h>
#include <Maths/vec.h>
#include <Maths/mat.h>

class Camera : public Transformable
{
public: 
    enum Type
    {
        CAMERA2D,
        CAMERA3D,
    };

private:
	inline static Camera* pActiveCamera = nullptr;
    Type iType;

protected:
    vec2 v2CurrentResolution;
	vec3 v3ActualPosition, v3ViewDirection, v3Up, v3StrafeDirection;
	mat4 mActiveProjectionMatrix, mOrthographicMatrix, mPerspectiveMatrix, mViewMatrix;
	mat3 mRotator;
	float fFOV = 80.0f, fZoomfactor = 1.0f, fZoomSpeed = 1.0f;
    float& fPitch = qRotation.x;
    float& fYaw = qRotation.y;
    float& fRoll = qRotation.z;

    void updateView();

public:
    Camera(const ivec2& resolution, const Type& type);
    ~Camera();

    virtual void update() {};
    virtual void updateProjection(const ivec2& resolution) {};
    void makeActive();
    void invertPitch();

    //Moving/Strafing
    void moveForward(const float& f);
    void moveBackward(const float& f);
    void moveLeft(const float& f);
    void moveRight(const float& f);
    void moveUp(const float& f);
    void moveDown(const float& f);


    //Getter
    mat4 getProjectionMatrix() const;
    mat4 getViewMatrix() const;
    mat4 getOrtho() const;
    mat4 getPerspective() const;
    vec3 getViewDirection() const;
    vec3 getPosition() const override;
    float getSpeed() const;
    float getFOV() const;
    Type getType() const;
    static Camera* getActiveCamera();
    

    //Setter
    void setZoomFactor(const float& factor);
    void setZoomSpeed(const float& speed);
    void setFOV(const float& fov);
};