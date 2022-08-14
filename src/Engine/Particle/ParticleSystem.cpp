#include "ParticleSystem.h"
#include <algorithm>
#include <gum-maths.h>

#include "../General/World.h"

#include <GL/glew.h>

ParticleSystem::ParticleSystem(World* world)
{
	pWorld = world;
	properties = new ParticleProperties();
	ParticleStages = new std::vector<Particle::ParticleStage*>();
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::create(std::string file)
{
	std::vector<float> vertices =
	{
		-0.5,  0.5, 0,
		-0.5, -0.5, 0,
		 0.5,  0.5, 0,
		 0.5,  0.5, 0,
		-0.5, -0.5, 0,
		 0.5, -0.5, 0
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VertexPositions);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VertexPositions);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &partPositionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, partPositionsVBO);
	glBufferData(GL_ARRAY_BUFFER, vParticles.size() * sizeof(Particle), vParticles.data(), GL_STREAM_DRAW);
	glEnableVertexAttribArray(11);
    glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, v3Position) + offsetof(vec3, x)));
	glVertexAttribDivisor(11, 1);

	glEnableVertexAttribArray(12);
    glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, v2Scale) + offsetof(vec2, x)));
    glVertexAttribDivisor(12, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	
	glGenBuffers(1, &TexOffsets);
	glBindBuffer(GL_ARRAY_BUFFER, TexOffsets);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(vec4), 0, GL_STREAM_DRAW);
	glEnableVertexAttribArray(13);
    glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (void*)offsetof(vec4, x));
	glVertexAttribDivisor(13, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	glGenBuffers(1, &TexCoordInfo);
	glBindBuffer(GL_ARRAY_BUFFER, TexCoordInfo);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(vec2), 0, GL_STREAM_DRAW);
	glEnableVertexAttribArray(14);
    glVertexAttribPointer(14, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)offsetof(vec2, x));
	glVertexAttribDivisor(14, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);
}

void ParticleSystem::render()
{
	if (vParticles.size() > 0)
	{
		glDisable(GL_CULL_FACE);

		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(11);
		glEnableVertexAttribArray(12);
		glEnableVertexAttribArray(13);
		glEnableVertexAttribArray(14);

		glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glActiveTexture(GL_TEXTURE0);
		texture->bind();
		glDepthMask(GL_FALSE);
		glEnable(GL_DEPTH_TEST);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, vParticles.size());

		texture->unbind();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(14);
		glDisableVertexAttribArray(13);
		glDisableVertexAttribArray(12);
		glDisableVertexAttribArray(11);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		glEnable(GL_CULL_FACE);
	}
}

void ParticleSystem::update()
{
	if(play)
	{
		for(int i = 0; i < properties->amount; i++)
		{
			vec3 randomOffset = vec3(Gum::Maths::randf(-properties->RandomPositionOffset.x, properties->RandomPositionOffset.x), Gum::Maths::randf(-properties->RandomPositionOffset.y, properties->RandomPositionOffset.y), Gum::Maths::randf(-properties->RandomPositionOffset.z, properties->RandomPositionOffset.z));
            addParticle(v3Position + randomOffset, properties->lifetime);
		}
	}

	SortParticles();

	for (size_t i = 0; i < vParticles.size(); i++)
	{
		if(play)
		{
			if(vParticles[i].update(properties->NumberOfRows, properties->NumberOfCollumns, ParticleStages, pWorld->getPhysics()->getWindDir()))
			{
				vParticles.erase(vParticles.begin() + i);
				TexCoordInfoVector.erase(TexCoordInfoVector.begin() + i);
				TexOffsetsVector.erase(TexOffsetsVector.begin() + i);
			}
			else
			{
				TexCoordInfoVector[i] = vec2(properties->NumberOfRows, vParticles[i].getBlend());
				TexOffsetsVector[i] = vec4(vParticles[i].getTexCoords1().x, vParticles[i].getTexCoords1().y, vParticles[i].getTexCoords2().x, vParticles[i].getTexCoords2().y);}
			}
	}
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, partPositionsVBO);
	glBufferData(GL_ARRAY_BUFFER, vParticles.size() * sizeof(Particle), vParticles.data(), GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, vParticles.size() * sizeof(Particle), vParticles.data());

	glBindBuffer(GL_ARRAY_BUFFER, TexOffsets);
	glBufferData(GL_ARRAY_BUFFER, TexOffsetsVector.size() * sizeof(vec4), TexOffsetsVector.data(), GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, TexOffsetsVector.size() * sizeof(vec4), TexOffsetsVector.data());

	glBindBuffer(GL_ARRAY_BUFFER, TexCoordInfo);
	glBufferData(GL_ARRAY_BUFFER, TexCoordInfoVector.size() * sizeof(vec2), TexCoordInfoVector.data(), GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, TexCoordInfoVector.size() * sizeof(vec2), &TexCoordInfoVector[0]);

	glBindVertexArray(0);
}

void ParticleSystem::addParticle(vec3 position, float lifetime)
{
	if(properties->maxamount > (int)vParticles.size())
	{
		float killtime;
		if(properties->RandomLifetime)
		{
			float randtime = Gum::Maths::randf(0, lifetime);
			if(randtime > properties->killtime)
			{
				killtime = properties->killtime;
			}
			else
			{
                killtime = randtime;
			}
		}
		else
		{
            killtime = lifetime;
		}

        Particle particle(position, killtime, lifetime);
		vParticles.push_back(particle);

		if (vParticles.size() > MAX_INSTANCES)
		{
			vParticles.erase(vParticles.begin());
			TexCoordInfoVector.erase(TexCoordInfoVector.begin());
			TexOffsetsVector.erase(TexOffsetsVector.begin());
		}

		TexCoordInfoVector.push_back(vec2(properties->NumberOfRows, particle.getBlend()));
		TexOffsetsVector.push_back(vec4(particle.getTexCoords1().x, particle.getTexCoords1().y, particle.getTexCoords2().x, particle.getTexCoords2().y));
	}
}

void ParticleSystem::addStage(float timeInPercent, float speed, vec3 velocity, vec3 randomness, vec2 scale)
{
    Particle::ParticleStage *stage = new Particle::ParticleStage();

    stage->timeInPercent = timeInPercent;
    stage->speed = speed;
    stage->velocity = velocity;
    stage->randomness = randomness;
    stage->scale = scale;

    ParticleStages->push_back(stage);
}


void ParticleSystem::Play()          { play = true; }
void ParticleSystem::Stop()          { play = false; }
void ParticleSystem::switchOnOff()   { play = !play; }
void ParticleSystem::SortParticles() { std::sort(&vParticles[0], &vParticles[0] + vParticles.size()); }


//
// Setter
//
void ParticleSystem::setTexture(Texture *texture)                          { this->texture = texture; }
void ParticleSystem::setPosition(vec3 pos)                                 { this->v3Position = pos; }
void ParticleSystem::setProperties(ParticleProperties *NewProperties)      { this->properties = NewProperties; }

//
// Getter
//
std::vector<Particle::ParticleStage*>* ParticleSystem::getParticleStages() { return ParticleStages; }
Texture* ParticleSystem::getTexture()                                      { return texture; }
vec3 ParticleSystem::getPosition()                                         { return v3Position; }
int ParticleSystem::getCount()                                             { return vParticles.size(); }
ParticleProperties* ParticleSystem::getProperties()                        { return properties; }