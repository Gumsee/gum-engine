#include "Particle.h"
#include "../General/Camera.h"

#include <Essentials/FPS.h>
#include <gum-maths.h>

Particle::Particle(vec3 pos, float lifetime, int maxlifetime)
{
	v3Position = pos;
	this->killtime = lifetime;
	this->lifeTime = maxlifetime;
}


Particle::~Particle() { }

bool Particle::update(int NumberOfRows, int NumberofCollumns, std::vector<Particle::ParticleStage*> *stages, vec3 winddir)
{
	elapsedTime += FPS::get();

	int Stageindex0 = Stageindex1 - 1;
	if(Stageindex0 < 0)
	{
		Stageindex0 = 0;
	}

	vec3 vel0 = stages->at(Stageindex0)->velocity;
	vec3 vel1 = stages->at(Stageindex1)->velocity;
	Velocity = vec3::mix(vel0, vel1, stageblendfactor);
	Velocity = vec3::normalize(Velocity);
	if(std::isnan(Velocity.x))
	{
		Velocity = vec3(0);
	}


	float speed0 = stages->at(Stageindex0)->speed;
	float speed1 = stages->at(Stageindex1)->speed;
	float finalspeed = Gum::Maths::mix(speed0, speed1, stageblendfactor);
	if(std::isnan(finalspeed))
	{
		finalspeed = 0;
	}
	
	vec3 FinalVelocity = (Velocity * finalspeed + randomAddition + winddir) * FPS::get();
	if(std::isnan(FinalVelocity.x))
	{
		FinalVelocity = vec3(0);
	}

	v3Position += FinalVelocity;

	vec2 scale0 = stages->at(Stageindex0)->scale;
	vec2 scale1 = stages->at(Stageindex1)->scale;
	vec2 finalscale = vec2::mix(scale0, scale1, stageblendfactor);
    if(std::isnan(finalscale.x) || std::isnan(finalscale.x))
    {
        finalscale = vec2(0);
    }
	this->v2Scale = finalscale;


	float lifeFactor = elapsedTime / lifeTime;
	int stageCount = stages->size();
	float Progression = lifeFactor * 100;
	//get the max value between two stages
	float max = stages->at(Stageindex1)->timeInPercent - stages->at(Stageindex0)->timeInPercent;

	//get progress between two stages
	float current = stages->at(Stageindex1)->timeInPercent - Progression;

	//get percentage between two stages
	stageblendfactor = current / max;
	//Flip value
	stageblendfactor = 1 - stageblendfactor;

	if(stageCount > Stageindex1)
	{
		if(Progression > stages->at(Stageindex1)->timeInPercent)
		{
			Stageindex1++;
			if(Stageindex1 > stageCount)
			{
				Stageindex1 = stageCount;
			}

			randomAddition = vec3(
				Gum::Maths::randf(-stages->at(Stageindex1 - 1)->randomness.x, stages->at(Stageindex1 - 1)->randomness.x),
				Gum::Maths::randf(-stages->at(Stageindex1 - 1)->randomness.y, stages->at(Stageindex1 - 1)->randomness.y),
				Gum::Maths::randf(-stages->at(Stageindex1 - 1)->randomness.z, stages->at(Stageindex1 - 1)->randomness.z)
			);
		}
	}

	updateTextureCoordInfo(NumberOfRows, NumberofCollumns);
	return elapsedTime > killtime;
}

void Particle::updateTextureCoordInfo(int NumberofRows, int NumberofCollumns)
{
	float lifeFactor = elapsedTime / lifeTime;
	int stageCount = NumberofRows * NumberofCollumns;
	float atlasProgression = lifeFactor * stageCount;
	int index1 = (int)floor(atlasProgression);
	int index2;
	if (index1 < stageCount - 1)
	{
		index2 = index1 + 1;
	}
	else
	{
		index2 = index1;
	}
	blend = std::modf(atlasProgression, new float(1.0f));

	int column = index1 % NumberofCollumns;
	int row = index1 / NumberofRows;
	TexCoords1.x = (float)column / NumberofCollumns;
	TexCoords1.y = (float)row / NumberofRows;

	column = index2 % NumberofCollumns;
	row = index2 / NumberofRows;
	TexCoords2.x = (float)column / NumberofCollumns;
	TexCoords2.y = (float)row / NumberofRows;
}

void Particle::setTextureOffset(vec2 &offset, int index, int NumberofRows, int NumberofCollumns)
{
	int column = index % NumberofCollumns;
	int row = index / NumberofRows;
	offset.x = (float)column / (float)NumberofCollumns;
	offset.y = (float)row / (float)NumberofRows;
}

float Particle::getcameradistance() 			{ return vec3::distance(v3Position, Camera::ActiveCamera->getPosition()); }
float Particle::getBlend() 						{ return blend; }
vec3 Particle::getPosition() 					{ return v3Position; }
vec2 Particle::getTexCoords1() 					{ return TexCoords1; }
vec2 Particle::getTexCoords2() 					{ return TexCoords2; }
bool Particle::operator<(Particle& that) 		{ return this->getcameradistance() > that.getcameradistance(); } // Sort in reverse order : far particles drawn first.

void Particle::setVelocity(vec3 newVelocity) 	{ this->Velocity = newVelocity; }
void Particle::increaseVelociy(vec3 var) 		{ this->Velocity += var; }
void Particle::setLifeTime(float lifetime) 		{ this->lifeTime = lifetime; }
void Particle::setScale(float scale) 			{ this->v2Scale = scale; }