#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
private:
	vec3 direction;
  
public:
	DirectionalLight(vec3 direction, vec3 color, std::string name);
	~DirectionalLight();

	vec3 *getDirection()
	{
		return &direction;
	}

	void setDirection(vec3 Direction)
	{
		this->direction = Direction;
	}
};

