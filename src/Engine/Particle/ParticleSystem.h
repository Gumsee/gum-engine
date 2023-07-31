#pragma once
#include <OpenGL/Texture.h>

#include "OpenGL/VertexArrayObject.h"
#include "OpenGL/VertexBufferObject.h"
#include "Particle.h"
#include "ParticleProperties.h"

class World;

class ParticleSystem
{
private:
	inline static const std::vector<float> vertices {
		-0.5,  0.5, 0,
		-0.5, -0.5, 0,
		 0.5,  0.5, 0,
		 0.5,  0.5, 0,
		-0.5, -0.5, 0,
		 0.5, -0.5, 0
	};

	std::vector<Particle::ParticleStage*> *ParticleStages;
	std::vector<Particle> vParticles;
	ParticleProperties *properties;
	const unsigned int MAX_INSTANCES = 100000;

	VertexArrayObject* pVAO;
	VertexBufferObject<vec4> *TexOffsets;
	VertexBufferObject<vec2> *TexCoordInfo;
	VertexBufferObject<Particle> *partPositionsVBO;
	Texture *texture;

	std::vector<vec2> TexCoordInfoVector;
	std::vector<vec4> TexOffsetsVector;

	World* pWorld;

	vec3 v3Position;
	bool play = true;

public:
	ParticleSystem(World* world);
	~ParticleSystem();

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