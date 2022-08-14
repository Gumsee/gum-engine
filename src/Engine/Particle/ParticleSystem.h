#pragma once
#include <OpenGL/Texture.h>

#include "Particle.h"
#include "ParticleProperties.h"

class World;

class ParticleSystem
{
private:
	std::vector<Particle::ParticleStage*> *ParticleStages;
	std::vector<Particle> vParticles;
	ParticleProperties *properties;
	const unsigned int MAX_INSTANCES = 100000;

	uint VAO;
	uint VertexPositions;
	uint TexOffsets;
	uint TexCoordInfo;
	uint partPositionsVBO;
	Texture *texture;

	std::vector<vec2> TexCoordInfoVector;
	std::vector<vec4> TexOffsetsVector;

	World* pWorld;

	vec3 v3Position;
	bool play = true;

public:
	ParticleSystem(World* world);
	~ParticleSystem();

	void create(std::string file);
	void render();
	void update();

	void addParticle(vec3 position, float lifetime);
	void addStage(float timeInPercent, float speed, vec3 velocity, vec3 randomness, vec2 scale);
	void Play();
	void Stop();
	void switchOnOff();
	void SortParticles();


    //Setter
	void setTexture(Texture *texture);
	void setObjScale(float scale);
	void setObjPos(vec3 Pos);
	void setPosition(vec3 pos);
	void setProperties(ParticleProperties *NewProperties);

    //Getter
	Texture* getTexture();
	vec3 getPosition();
	int getCount();
	ParticleProperties *getProperties();
	std::vector<Particle::ParticleStage*> *getParticleStages();
};