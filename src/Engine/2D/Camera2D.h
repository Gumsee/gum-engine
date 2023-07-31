// #pragma once
// #include <Primitives/Transformable.h>
// #include <Essentials/SmoothFloat.h>
// #include <Maths/vec.h>
// #include <Maths/mat.h>

// class World2D;
// struct Object3DInstance;

// class Camera : public Transformable
// {
// public:
//     enum Modes
//     {
//         THIRDPERSON,
//         FREECAM,
//         FIRSTPERSON,
//         STATIC,
//         CONTROLLED
//     };
//     enum ProjectionModes
//     {
//         PERSPECTIVE3D,
//         ORTHOGRAPHIC3D,
//         ORTHOGRAPHIC2D
//     };

// private:
//     float ROTATIONAL_SPEED;
// 	float MOVEMENT_SPEED = 20.0f;
// 	float ZOOMFACTOR = 1.0f;

//     Object3DInstance* pCameraObjectInstance = nullptr;
// 	SmoothFloat* OffsetToPos;
// 	vec3 v3ActualPosition;
// 	vec3 v3ViewDirection;
// 	vec3 UP;
// 	vec3 strafeDirection;
// 	mat4 mActiveProjectionMatrix, mOrthographicMatrix, mPerspectiveMatrix;
// 	mat3 rotator;
// 	mat4 viewMat;
// 	float MouseAngle = 0;
// 	float AngleAroundPos = 0;
// 	float FOV = 80.0f;
//     float& fPitch = qRotation.x;
//     float& fYaw = qRotation.y;
//     float& fRoll = qRotation.z;
// 	bool isStopped = false;
// 	Modes iCurrentMode;
//     ProjectionModes iProjectionMode;
//     vec2 v2CurrentResolution;

//     void mouseUpdate();
//     void updateView();

// public:
//     Camera(const ivec2& resolution, World2D* world);
//     ~Camera();

// 	inline static Camera* ActiveCamera = nullptr;

//     void lookAt(const vec3& lookat);
//     void update();
//     void updateProjection(const ivec2& resolution);

    
//     void moveForward();
//     void moveBackward();
//     void strafeLeft();
//     void strafeRight();
//     void moveUp();
//     void moveDown();
//     void IncreaseSpeed(const float& speed);
//     void IncreasePosition(const float& X, const float& Y, const float& Z);
//     void invertPitch();
//     void stop();
//     void start();


//     //Setter
//     void setProjectionMode(const ProjectionModes& mode);
//     void setMode(const Modes& Mode);
//     void setOffset(const float& offset);
//     void setCarPosition(const vec3& pos);
//     void setIsInsideCar(const bool& val);
//     void setZoomFactor(const int& factor);
//     void setSpeed(const float& speed);
//     void setRotationalSpeed(const float& speed);


//     //Getter
//     vec3 getPosition() const override;
//     mat4 getProjectionMatrix() const;
//     mat4 getViewMatrix() const;
//     mat4 getOrtho() const;
//     mat4 getPerspective() const;
//     vec3 getViewDirection() const;
//     float getSpeed() const;
//     float getFOV() const;
//     Modes getMode() const;
//     ProjectionModes getProjectionMode() const;
// };
