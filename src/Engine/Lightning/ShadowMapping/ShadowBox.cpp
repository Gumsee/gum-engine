#include "ShadowBox.h"
#include "Engine/General/Camera.h"



ShadowBox::ShadowBox(Renderer3D* renderer)
{
	pRenderer = renderer;
}


ShadowBox::~ShadowBox()
{
}


/**
* Creates a new shadow box and calculates some initial values relating to
* the camera's view frustum, namely the width and height of the near plane
* and (possibly adjusted) far plane.
*
* @param lightViewMatrix
*            - basically the "view matrix" of the light. Can be used to
*            transform a point from world space into "light" space (i.e.
*            changes a point's coordinates from being in relation to the
*            world's axis to being in terms of the light's local axis).
* @param camera
*            - the in-game camera.
*/
void ShadowBox::create(mat4 *lightViewMatrix) {
	this->lightViewMatrix = lightViewMatrix;
	calculateWidthsAndHeights();
}

/**
* Updates the bounds of the shadow box based on the light direction and the
* camera's view frustum, to make sure that the box covers the smallest area
* possible while still ensuring that everything inside the camera's view
* (within a certain range) will cast shadows.
*/
void ShadowBox::update() 
{
	mat3 camrot; 

	mat4 rotation;
	rotation = camrot;
	vec3 forwardVector = mat4::transpose(rotation) * FORWARD;

	vec3 toFar = vec3(forwardVector);
	toFar *= (SHADOW_DISTANCE);
	vec3 toNear = forwardVector;
	toNear *= 0.1;
	vec3 centerNear = toNear + Camera::ActiveCamera->getPosition();
	vec3 centerFar = toFar + Camera::ActiveCamera->getPosition();

	vec3 upVector = mat4::transpose(rotation) * UP;
	vec3 rightVector = vec3::cross(forwardVector, upVector);
	vec3 downVector = vec3(-upVector.x, -upVector.y, -upVector.z);
	vec3 leftVector = vec3(-rightVector.x, -rightVector.y, -rightVector.z);
	vec3 farTop     = centerFar  + vec3(upVector.x   * farHeight,  upVector.y   * farHeight,  upVector.z   * farHeight);
	vec3 farBottom  = centerFar  + vec3(downVector.x * farHeight,  downVector.y * farHeight,  downVector.z * farHeight);
	vec3 nearTop    = centerNear + vec3(upVector.x   * nearHeight, upVector.y   * nearHeight, upVector.z   * nearHeight);
	vec3 nearBottom = centerNear + vec3(downVector.x * nearHeight, downVector.y * nearHeight, downVector.z * nearHeight);
	vec4 points[8];
	points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
	points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
	points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
	points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
	points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
	points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
	points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
	points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);

	bool first = true;
	for (int i = 0; i < 8; i++) 
	{
		if (first) {
			minX = points[i].x;
			maxX = points[i].x;
			minY = points[i].y;
			maxY = points[i].y;
			minZ = points[i].z;
			maxZ = points[i].z;
			first = false;
		}
		if (points[i].x > maxX) {
			maxX = points[i].x;
		}
		else if (points[i].x < minX) {
			minX = points[i].x;
		}
		if (points[i].y > maxY) {
			maxY = points[i].y;
		}
		else if (points[i].y < minY) {
			minY = points[i].y;
		}
		if (points[i].z > maxZ) {
			maxZ = points[i].z;
		}
		else if (points[i].z < minZ) {
			minZ = points[i].z;
		}
	}
	//maxZ += OFFSET;

}

/**
* Calculates the center of the "view cuboid" in light space first, and then
* converts this to world space using the inverse light's view matrix.
*
* @return The center of the "view cuboid" in world space.
*/
vec3 ShadowBox::getCenter() {
	float x = (minX + maxX) / 2.0f;
	float y = (minY + maxY) / 2.0f;
	float z = (minZ + maxZ) / 2.0f;
	vec4 cen = vec4(x, y, z, 1);
	mat4 invertedLight = mat4::inverse(*lightViewMatrix);;
	return vec3(mat4::transpose(invertedLight) * cen);
}

/**
* @return The width of the "view cuboid" (orthographic projection area).
*/
float ShadowBox::getWidth() {
	return maxX - minX;
}

/**
* @return The height of the "view cuboid" (orthographic projection area).
*/
float ShadowBox::getHeight() {
	return maxY - minY;
}

/**
* @return The length of the "view cuboid" (orthographic projection area).
*/
float ShadowBox::getLength() {
	return maxZ - minZ;
}
vec4 ShadowBox::calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width) {
	vec3 point = startPoint + vec3(direction.x * width, direction.y * width, direction.z * width);
	vec4 point4f = vec4(point.x, point.y, point.z, 1.0f);
	point4f = mat4::transpose(*lightViewMatrix) * point4f;
	return point4f;
}
mat4 ShadowBox::calculateCameraRotationMatrix() {
	//mat4 rotation;
	//mat4 rot1 = rotate((float)radians(-cam->getRotation().z), vec3(0, 1, 0));
	//mat4 rot2 = rotate((float)radians(-cam->getRotation().y), vec3(1, 0, 0));
	//rotation = rot1 * rot2;
	//return rotation;
}

/**
* Calculates the width and height of the near and far planes of the
* camera's view frustum. However, this doesn't have to use the "actual" far
* plane of the view frustum. It can use a shortened view frustum if desired
* by bringing the far-plane closer, which would increase shadow resolution
* but means that distant objects wouldn't cast shadows.
*/
void ShadowBox::calculateWidthsAndHeights() 
{
	farWidth = (float)(SHADOW_DISTANCE * Gum::Maths::tandeg(80.0f));
	nearWidth = (float)(0.1f * Gum::Maths::tandeg(80.0f));
	
	float fAspectRatioWidthToHeight = (float)pRenderer->getRenderCanvas()->getSize().x / (float)pRenderer->getRenderCanvas()->getSize().y;
	farHeight = farWidth / fAspectRatioWidthToHeight;
	nearHeight = nearWidth / fAspectRatioWidthToHeight;
}