#pragma once
#include <string>
#include "../Particle/Billboard.h"

class Light
{
protected:
	vec3 vPosition;
	vec3 vColor;
	vec3 vAttenuation;
	std::string sName;
	Billboard* pBillboard;
	float fStrength;
	
public:
	Light();
	~Light();

	void IncreasePosition(const vec3& pos);

	//Setter
	void setPosition(const vec3& pos);
	void setAttenuation(const vec3& att);
	void setStrength(const float& strength);
	void setColor(const vec3& rgb);
	void setName(const std::string& name);
	void setBillboard(Billboard* billboard);

	//Getter
	vec3 getColor() const;
	std::string getName() const;
	vec3 getPosition() const;
	vec3 getAttenuation() const;
	float getStrength() const;
};

