#include "Light.h"
#include <iostream>

Light::Light()
{
	this->vPosition = vec3();
	this->vColor = vec3(1.0f,1.0f,1.0f);
	this->vAttenuation = vec3(0.3, 0, 0.005);
	this->fStrength = 1;
	this->pCallback = nullptr;
}

Light::~Light() 
{

}

void Light::IncreasePosition(const vec3& pos) { setPosition(getPosition() + pos); }

void Light::update()
{
	if(this->pCallback != nullptr) 
	{ 
		this->pCallback(this); 
	}
}


//
// Setter
//
void Light::setPosition(const vec3& pos) 		          { this->vPosition = pos; update(); }
void Light::setAttenuation(const vec3& att) 	          { this->vAttenuation = att; update(); }
void Light::setStrength(const float& strength) 	          { this->fStrength = strength; update(); }
void Light::setColor(const vec3& rgb) 		              { this->vColor = rgb; update(); }
void Light::setName(const std::string& name) 	          { this->sName = name; }
void Light::setCallback(std::function<void(Light*)> func) { this->pCallback = func; update(); }


//
// Getter
//
vec3 Light::getPosition() const 	           	          { return this->vPosition; }
vec3 Light::getAttenuation() const          	          { return this->vAttenuation; }
float Light::getStrength() const 	    		          { return this->fStrength; }
std::string Light::getName() const 	    		          { return this->sName; }
vec3 Light::getColor() const 		        	          { return this->vColor; }