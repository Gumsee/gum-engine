#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(vec3 direction, vec3 color, std::string name)
  : direction(direction)
{
  this->setName("DirectionalLight_" + name);
	this->setColor(color);
	#ifdef DEBUG
		//this->setName("DirectionalLight" + name);

		//Billboard *billboard = new Billboard(vec3(1));
		//billboard->setTexture(GumManagers::Textures->getTexture("Sun.png"));
		//GumManagers::Particles->addBillboard(billboard, this->getName());
	#endif
}


DirectionalLight::~DirectionalLight()
{
}