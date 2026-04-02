#pragma once
#include <Primitives/Transformable.h>
#include <Essentials/SmoothFloat.h>
#include <Maths/vec.h>
#include <Maths/mat.h>
#include <functional>


class Camera : public Transformable<float, 3U>
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
	vec3 v3ActualPosition, v3ViewDirection, v3WorldUp, v3WorldFront, v3WorldUpFrontCross, v3StrafeDirection;
	mat4 mActiveProjectionMatrix, mOrthographicMatrix, mPerspectiveMatrix, mViewMatrix;
	mat3 mRotator;
	  float fFOV = 80.0f, fZoomfactor = 1.0f, fZoomSpeed = 1.0f, fMovementSpeed = 20.0f, fAspectRatio = 0.0f;
    float& fPitch = qRotation.x;
    float& fYaw = qRotation.y;
    float& fRoll = qRotation.z;
    static std::function<void()> pOnViewUpdate;
    
    Camera(const ivec2& resolution, const Type& type);

public:
    virtual ~Camera();

    virtual void update() = 0;
    virtual void updateProjection([[maybe_unused]] const ivec2& resolution) {};
    void updateView();
    void makeActive();
    void invertPitch();
		vec3 calcMouseRayDirection();		

    //Moving/Strafing
    void moveForward(const float& f);
    void moveBackward(const float& f);
    void moveLeft(const float& f);
    void moveRight(const float& f);
    void moveUp(const float& f);
    void moveDown(const float& f);


    //Getter
    vec3 getUpDirection() const;
    vec3 getFrontDirection() const;
    mat4 getProjectionMatrix() const;
    mat4 getViewMatrix() const;
    mat4 getOrtho() const;
    mat4 getPerspective() const;
    vec3 getViewDirection() const;
    vec3 getStrafeDirection() const;
    vec3& getPosition() override;
    float getFOV() const;
    Type getType() const;
    float getZoomSpeed() const;
    virtual float getZoom() const;
    static Camera* getActiveCamera();
    

    //Setter
    void setZoomFactor(const float& factor);
    void setZoomSpeed(const float& speed);
    void setFOV(const float& fov);
    void setAspectRatio(const float& aspect);
    void overrideViewMatrix(mat4 matrix);
    void setMovementSpeed(const float& speed);
    void setWorldUpDirection(const vec3& up);
    void setWorldFrontDirection(const vec3& front);

    static void onViewUpdate(std::function<void()> callback);
};