#pragma once
#include "Engine/3D/Object/SceneObject.h"
#include <gum-engine.h>

World3D* createBasicCubeExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true); //PREFILTER MAP RENDERN BEI GRADIANT
	//pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* light = new PointLight(vec3(-10, 1, 0), vec3(30), "light");
    pWorld3D->getLightManager()->addPointLight(light);


    SceneObject *cube = new SceneObject(Mesh::generateCube(vec3(2)), "cube");
    cube->addInstance();
    pWorld3D->getObjectManager()->addObject(cube);

    return pWorld3D;
}