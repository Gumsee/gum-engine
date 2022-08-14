#pragma once
#include <Maths/vec.h>
#include <Maths/mat.h>

class ShadowBox
{
private:
	const float OFFSET = 10;
	vec4 UP = vec4(0, 1, 0, 0);
	vec4 FORWARD = vec4(0, 0, -1, 0);
	const float SHADOW_DISTANCE = 20;

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	mat4 *lightViewMatrix;
	float farHeight, farWidth, nearHeight, nearWidth;
    
public:
	ShadowBox();
	~ShadowBox();

	void create(mat4 *lightViewMatrix);
	void update();
	vec3 getCenter();
	float getHeight();
	float getWidth();
	float getLength();
	void calculateWidthsAndHeights();
	mat4 calculateCameraRotationMatrix();
	vec4 calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width);
};

