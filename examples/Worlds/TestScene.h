#pragma once
#include <gum-engine.h>
#include <Engine/3D/Object/SceneObject.h>
#include <Engine/3D/World3D.h>

World3D* createTestSceneExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true); //PREFILTER MAP RENDERN BEI GRADIANT
	//pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* light = new PointLight(vec3(0, 0, 0), vec3(3000), "light");
    pWorld3D->getLightManager()->addPointLight(light);


    SceneObject *cube = new SceneObject(ObjectManager::MODEL_ASSETS_PATH + "/house.dae", "cube");
    cube->addInstance();
    pWorld3D->getObjectManager()->addObject(cube);

    return pWorld3D;
}