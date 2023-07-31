#pragma once
#include "Engine/Texture/TextureManager.h"
#include "Engine/Particle/ParticleSystem.h"
#include <Engine/Particle/ParticleProperties.h>
#include <gum-engine.h>

World3D* createParticlesExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));


	ParticleSystem *particles = new ParticleSystem(pWorld3D);
    // "1", GumEngine::Textures->getTexture("particleAtlas2.png"), "ParticleSystem"

    particles->setTexture(Gum::TextureManager::getTexture("particleAtlas.png", true));

	ParticleProperties *properties = particles->getProperties();
	properties->lifetime = 4;
	properties->killtime = 4;
	properties->amount = 10;
	properties->maxamount = 10000;
	properties->NumberOfRows = 4;
	properties->NumberOfCollumns = 4;
	properties->RandomLifetime = false;
//   properties->RandomPositionOffset = vec3(2, 0, 2);
	properties->Transparency = true;

	particles->addStage(0,   2, vec3(0,3,0), vec3(1), vec2(1));
	particles->addStage(50,  5, vec3(0,3,0), vec3(2), vec2(2));
	particles->addStage(100, 5, vec3(0,3,0), vec3(3), vec2(2));

    pWorld3D->addParticles(particles);

	//particles->addStage(20, 2, vec3(0,0,1), vec3(4), vec2(2));
	//particles->addStage(100, 1, vec3(10,10,10), vec3(0), vec2(0));

	return pWorld3D;
}
