#pragma once
#include "Light.h"

class SpotLight : public Light
{
private:
	vec3 direction;
public:
	SpotLight(vec3 Position, vec3 Direction, float strengh);
	~SpotLight();

	vec3 getDirection() 
	{
		return this->direction;
	}

	void setDirection(vec3 Direction)
	{
		this->direction = Direction;
	}
};

