#pragma once
#include "../Rendering/Camera.h"
#include <Essentials/SmoothFloat.h>
#include <Maths/vec.h>
#include <Maths/mat.h>

class World3D;
struct Object3DInstance;

class Camera3D : public Camera
{
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

private:
    inline static const float ROTATIONAL_SPEED = 0.3f;
	SmoothFloat* pOffsetToPos;
	float fMouseAngle;
	float fAngleAroundPos;
	Modes iCurrentMode;
    ProjectionModes iProjectionMode;

    void mouseUpdate();

public:
    Camera3D(const ivec2& resolution, World3D* world);
    ~Camera3D();

	inline static Camera* ActiveCamera = nullptr;

    void lookAt(const vec3& lookat);
    void update() override;
    void updateProjection(const ivec2& resolution) override;


    //Setter
    void setProjectionMode(const ProjectionModes& mode);
    void setMode(const Modes& Mode);
    void setOffset(const float& offset);


    //Getter
    Modes getMode() const;
    ProjectionModes getProjectionMode() const;
};
