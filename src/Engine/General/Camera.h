#pragma once
#include <Essentials/Window.h>
#include <Essentials/SmoothFloat.h>
#include <Maths/vec.h>
#include <Maths/mat.h>

class World;
struct Instance;

class Camera
{
private:
    float ROTATIONAL_SPEED;
	float MOVEMENT_SPEED = 20.0f;
	int ZOOMFACTOR = 1;

    Instance* pCameraObjectInstance = nullptr;
	SmoothFloat* OffsetToPos;
	vec3 v3Position, v3ActualPosition;
	vec3 v3ViewDirection;
	vec3 UP;
	vec3 strafeDirection;
	mat4 projectionMatrix;
	mat4 OrthographicMatrix;
	mat3 rotator;
	mat4 viewMat;
	float MouseAngle = 0;
	float fPitch = 0;
	float fYaw = 0;
	float fRoll = 0;
	float AngleAroundPos = 0;
	float FOV = 80.0f;
	bool isStopped = false;
	int CurrentMode = 0;
    unsigned int iProjectionMode;
    vec2 v2CurrentResolution;
    Gum::Window* pContextWindow;

    void mouseUpdate();
    void updateView();

public:
    enum Modes
    {
        THIRDPERSON,
        FREECAM,
        FIRSTPERSON,
        STATIC,
        CONTROLLED
    };
    enum ProjectionModes
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    Camera(const ivec2& resolution, World* world, Gum::Window* context);
    ~Camera();

	static Camera* ActiveCamera;

    void lookAt(const vec3& lookat);
    void update();
    void updateProjection(const ivec2& resolution);

    
    void moveForward();
    void moveBackward();
    void strafeLeft();
    void strafeRight();
    void moveUp();
    void moveDown();
    void IncreaseSpeed(const float& speed);
    void IncreasePosition(const float& X, const float& Y, const float& Z);
    void invertPitch();
    void stop();
    void start();
    void setProjectionMode(const unsigned int& mode);


    //Setter
    void setMode(const int& Mode);
    void setRotation(const vec3& rot);
    void setOffset(const float& offset);
    void setCarPosition(const vec3& pos);
    void setIsInsideCar(const bool& val);
    void setZoomFactor(const int& factor);
    void setSpeed(const float& speed);
    void setPosition(const vec3& pos);


    //Getter
    vec3 getRotation() const;
    mat4 getProjectionMatrix() const;
    mat4 getViewMatrix() const;
    mat4 getOrtho() const;
    vec3 getPosition() const;
    vec3 getViewDirection() const;
    float getSpeed() const;
    int getMode() const;
    unsigned int getProjectionMode() const;
};
