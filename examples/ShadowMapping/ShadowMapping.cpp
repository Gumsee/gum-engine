#include <Engine.h>
#include <Extras/Terrain.h>
#include <Maths/Math.h>
#include <Object/Object.h>
#include "../Examples.h"

World* Examples::createShadowMappingExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

	GumEngine::ActiveCamera->setMode(Camera::FREECAM);

	pWorld->getObjectManager()->getSkybox()->useGradiant(false); //PREFILTER MAP RENDERN BEI GRADIANT
	pWorld->getObjectManager()->getSkybox()->setTexture(GumEngine::Textures->getTexture("Sky/spaichingen_hill_8k.hdr", true));

	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 10),  vec3(300), "light1"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 10),   vec3(300), "light2"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 10), vec3(300), "light3"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 10),  vec3(300), "light4"));

    Terrain* pTerrain = new Terrain(vec2(500), ivec2(100, 100), 40, 69);
    Object* pTerrainObj = new Object(pTerrain, "TestTerrain");
    pTerrainObj->setPosition(vec3(-pTerrain->getSize().x * 0.5f, 0, -pTerrain->getSize().y * 0.5f), 0);
	pTerrainObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("grass.jpg"), 0);
	pTerrainObj->getMaterial()->setTextureMultiplier(10);
    pWorld->getObjectManager()->addObject(pTerrainObj);

    Mesh *pBoxMesh = Mesh::generateCube(vec3(1,1,1));
	Object* pCubeObject = new Object(pBoxMesh, "Cube");

	for(int i = 0; i < 300; i++)
	{
		pCubeObject->addInstance();
		vec2 pos(
			Math::randf(-pTerrain->getSize().x * 0.5f, pTerrain->getSize().x * 0.5f), 
			Math::randf(-pTerrain->getSize().y * 0.5f, pTerrain->getSize().y * 0.5f)
		);
		float scale = Math::randf(1, 4);
		pCubeObject->setPosition(vec3(
			pos.x,
			pTerrain->getHeight(pos.x+pTerrain->getSize().x * 0.5f, pos.y+pTerrain->getSize().y * 0.5f) + scale * 0.5f,
			pos.y
		));
		pCubeObject->setScale(scale);
	}
    pWorld->getObjectManager()->addObject(pCubeObject);
	
	return pWorld;
}
