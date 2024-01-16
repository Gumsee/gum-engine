#pragma once
#include <Engine/3D/Object/NormalRenderer/NormalRenderer.h>
#include "Engine/3D/Object/SceneObject.h"
#include <Engine/3D/Object/Skeletal/AnimatedModel.h>
#include <gum-engine.h>

World3D* createAnimatedModelExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

    //Default
    /*AnimatedModel* pAnimatedModel = new AnimatedModel(ObjectManager::MODEL_ASSETS_PATH + "/animTest.dae", "AnimTest");
    pAnimatedModel->addInstance();

	SkeletalAnimation* pWalkCycle = new SkeletalAnimation("WalkCycle", vec2(1, 10), 2, 0.1, 0);
    pAnimatedModel->PlaySkeletalAnimation(pWalkCycle, true);

    pWorld3D->getObjectManager()->addObject(pAnimatedModel);*/

	SceneObject* pTestOBJ = new SceneObject(Mesh::generateCube(vec3(1,1,1)), "TestOBJ");
    pTestOBJ->addInstance();
    pWorld3D->getObjectManager()->addObject(pTestOBJ);

    pWorld3D->addRenderable(new NormalRenderer(pWorld3D));

	return pWorld3D;
}