#include "SpotLight.h"



SpotLight::SpotLight(vec3 Position, vec3 Direction, float strengh)
{
	this->direction = Direction;
	this->setPosition(Position);
	this->setStrength(strengh);
}


SpotLight::~SpotLight()
{
}