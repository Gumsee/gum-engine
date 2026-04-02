#include "PointLight.h"

PointLight::PointLight(vec3 Position, vec3 color, std::string name)
{
  this->setName("PointLight_" + name);
	this->setPosition(Position);
	this->setColor(color);
}	


PointLight::~PointLight() { }