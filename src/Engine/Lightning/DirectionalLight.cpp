#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(vec3 Direction, vec3 color, std::string name)
{
	this->direction = Direction;
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