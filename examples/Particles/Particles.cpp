#include <Engine.h>
#include "../Examples.h"

World* Examples::createParticlesExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));


	/*ParticleSystem *particles = GumEngine::DefaultRenderer->addParticleSystem("1", GumEngine::Textures->getTexture("particleAtlas2.png"), "ParticleSystem");

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
	particles->addStage(100, 5, vec3(0,3,0), vec3(3), vec2(2));*/

	//particles->addStage(20, 2, vec3(0,0,1), vec3(4), vec2(2));
	//particles->addStage(100, 1, vec3(10,10,10), vec3(0), vec2(0));

	return pWorld;
}
