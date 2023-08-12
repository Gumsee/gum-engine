#pragma once
#include "../Rendering/Camera.h"
#include <Essentials/SmoothFloat.h>
#include <Maths/vec.h>
#include <Maths/mat.h>

class World2D;

class Camera2D : public Camera
{
private:
    inline static const float ROTATIONAL_SPEED = 0.3f;
	SmoothFloat* pOffsetToPos;
	float fMouseAngle;
	float fAngleAroundPos;

    void mouseUpdate();

public:
    Camera2D(const ivec2& resolution, World2D* world);
    ~Camera2D();

    void update() override;
    void updateProjection(const ivec2& resolution) override;


    //Setter


    //Getter
};
