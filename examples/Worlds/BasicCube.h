#pragma once
#include "Engine/3D/Object/SceneObject.h"
#include <gum-engine.h>

World3D* createBasicCubeExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true); //PREFILTER MAP RENDERN BEI GRADIANT
	//pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* pSpinningLight = new PointLight(vec3(10, 1, 0),   vec3(30), "light2");
    pWorld3D->getLightManager()->addPointLight(pSpinningLight);


    SceneObject *cube = new SceneObject(Mesh::generateCube(vec3(10)), "RoughBronze");
    cube->addPhysicsInstance(PhysicsObjectInstance::BOX, 1.0f, pWorld3D);
	//cube->getMaterial()->setColor(vec4(1,0,0,1));
	//cube->setPosition(vec3(0, 0, 0));
    //RoughBronzeObj->getInstance()->renderOutline = true;
    pWorld3D->getObjectManager()->addObject(cube);


    return pWorld3D;
}