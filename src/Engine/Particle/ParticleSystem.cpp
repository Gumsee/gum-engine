#include "ParticleSystem.h"
#include <algorithm>
#include <gum-maths.h>

#include "../Rendering/World.h"
#include "Graphics/Graphics.h"
#include "Graphics/Variables.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/VertexBufferObject.h"
#include "Particle.h"
#include "System/Output.h"

ParticleSystem::ParticleSystem(World* world)
{
	pWorld = world;
	properties = new ParticleProperties();
	ParticleStages = new std::vector<Particle::ParticleStage*>();

    pVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP);

    VertexBufferObject<float> vertexBuffer;
    vertexBuffer.setData(vertices, Gum::Graphics::DataState::STATIC);
    pVAO->addAttribute(&vertexBuffer, 0, 3, Gum::Graphics::Datatypes::FLOAT, 0, 0);
    
    partPositionsVBO = new VertexBufferObject<Particle>();
    partPositionsVBO->setData(vParticles, Gum::Graphics::DataState::DYNAMIC);
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Winvalid-offsetof"
    pVAO->addAttribute(partPositionsVBO, 1, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(Particle), offsetof(Particle, v3Position), 1);
    pVAO->addAttribute(partPositionsVBO, 2, 2, Gum::Graphics::Datatypes::FLOAT, sizeof(Particle), offsetof(Particle, v2Scale), 1);
    #pragma GCC diagnostic pop
	
    TexOffsets = new VertexBufferObject<vec4>();
    pVAO->addAttribute(TexOffsets, 3, 4, Gum::Graphics::Datatypes::FLOAT, sizeof(vec4), 0, 1);
	
    TexCoordInfo = new VertexBufferObject<vec2>();
    pVAO->addAttribute(TexCoordInfo, 4, 2, Gum::Graphics::Datatypes::FLOAT, sizeof(vec2), 0, 1);
    pVAO->setVertexCount(6);
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::render()
{
	if (vParticles.size() > 0)
	{
    Gum::Graphics::disableFeature(Gum::Graphics::Features::CULL_FACE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glDepthMask(GL_FALSE);

    pVAO->bind();
		texture->bind();
    pVAO->render((unsigned int)vParticles.size());
		texture->unbind();
    pVAO->unbind();

		//glDepthMask(GL_TRUE);
    Gum::Graphics::enableFeature(Gum::Graphics::Features::CULL_FACE);
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
	
    partPositionsVBO->setData(vParticles, Gum::Graphics::DataState::DYNAMIC);
    TexOffsets->setData(TexOffsetsVector, Gum::Graphics::DataState::DYNAMIC);
    TexCoordInfo->setData(TexCoordInfoVector, Gum::Graphics::DataState::DYNAMIC);
}

void ParticleSystem::addParticle(vec3 position, float lifetime)
{
	if(properties->maxamount > (int)vParticles.size())
	{
		float killtime;
		if(properties->RandomLifetime)
		{
			float randtime = Gum::Maths::randf(0, lifetime);
      killtime = randtime > properties->killtime 
        ? properties->killtime 
        : randtime;
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
void ParticleSystem::SortParticles() { std::sort(vParticles.begin(), vParticles.end(), [](Particle a, Particle b) -> bool { 
    return a.getcameradistance() > b.getcameradistance();
}); }


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
unsigned int ParticleSystem::getCount()                                    { return (unsigned int)vParticles.size(); }
ParticleProperties* ParticleSystem::getProperties()                        { return properties; }
