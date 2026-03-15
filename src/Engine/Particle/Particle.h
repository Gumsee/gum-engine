#pragma once
#include <vector>
#include <Maths/vec.h>
#include <Maths/mat.h>

class Particle
{
private:
	float lifeTime = 1;
	float killtime = 1;
	float elapsedTime = 0;
	vec3 Velocity, randomAddition;

	int Stageindex1 = 0;
	float stageblendfactor = 0;

	vec2 TexCoords1, TexCoords2;
	float blend = 0;


public:
	vec3 v3Position;
	vec3 v3Rotation;
	vec2 v2Scale;



	Particle(vec3 pos, float lifetime, float maxlifetime);
	~Particle();

	struct ParticleStage
	{
		float timeInPercent;
		vec3 velocity;
		float speed = 1.0f;
		vec3 randomness;	
		vec2 scale;
	};

	bool update(int NumberOfRows, int NumberofCollumns, std::vector<Particle::ParticleStage*> *stages, vec3 winddir);

	void updateTextureCoordInfo(int NumberofRows, int NumberofCollumns);
	void setTextureOffset(vec2 &offset, int index, int NumberofRows, int NumberofCollumns);
	void setVelocity(vec3 newVelocity);
	void increaseVelociy(vec3 var);
	void setLifeTime(float lifetime);
	void setScale(float scale);

	vec3 getPosition();
	float getBlend();
	vec2 getTexCoords1();
	vec2 getTexCoords2();
	mat4 *getTransformationMatrix();
	float getcameradistance();
	bool operator<(Particle& that);
};