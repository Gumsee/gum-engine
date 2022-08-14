#pragma once
#include "Light.h"

class PointLight : public Light
{
private:
public:
	PointLight(vec3 Position, vec3 color, std::string name);
	~PointLight();
};

