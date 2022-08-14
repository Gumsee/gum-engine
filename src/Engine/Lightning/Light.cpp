#include "Light.h"
#include <iostream>

Light::Light()
{
	this->vPosition = vec3();
	this->vColor = vec3(1.0f,1.0f,1.0f);
	this->vAttenuation = vec3(0.3, 0, 0.005);
	this->fStrength = 1;
	this->pBillboard = nullptr;
}

Light::~Light() { }

void Light::IncreasePosition(const vec3& pos) { this->vPosition += pos; }

//
// Setter
//
void Light::setPosition(const vec3& pos) 		{ this->vPosition = pos; if(this->pBillboard != nullptr) {this->pBillboard->setPosition(pos);} }
void Light::setAttenuation(const vec3& att) 	{ this->vAttenuation = att; }
void Light::setStrength(const float& strength) 	{ this->fStrength = strength; }
void Light::setColor(const vec3& rgb) 		    { this->vColor = rgb; }
void Light::setName(const std::string& name) 	{ this->sName = name; }
void Light::setBillboard(Billboard* billboard) 	{ this->pBillboard = billboard; }


//
// Getter
//
vec3 Light::getPosition() const 	           	{ return this->vPosition; }
vec3 Light::getAttenuation() const          	{ return this->vAttenuation; }
float Light::getStrength() const 	    		{ return this->fStrength; }
std::string Light::getName() const 	    		{ return this->sName; }
vec3 Light::getColor() const 		        	{ return this->vColor; }