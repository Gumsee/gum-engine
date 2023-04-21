#pragma once
#include "Engine/General/World.h"
#include "Engine/Managers/TextureManager.h"
#include <gum-engine.h>

World* createBasicCubeExample()
{
    World* pWorld = new World();
	pWorld->getObjectManager()->getSkybox()->useGradiant(true); //PREFILTER MAP RENDERN BEI GRADIANT
	//pWorld->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* pSpinningLight = new PointLight(vec3(10, 1, 0),   vec3(30), "light2");
    pWorld->getLightManager()->addPointLight(pSpinningLight);


    Object *cube = new Object(Mesh::generateCube(vec3(10)), "RoughBronze");
	//cube->getMaterial()->setColor(vec4(1,0,0,1));
	//cube->setPosition(vec3(0, 0, 0));
    //RoughBronzeObj->getInstance()->renderOutline = true;
    pWorld->getObjectManager()->addObject(cube);


    return pWorld;
}