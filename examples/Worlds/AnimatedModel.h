#pragma once
#include <Engine/3D/Object/Skeletal/AnimatedModel.h>
#include <gum-engine.h>

World3D* createAnimatedModelExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

    //Default
    //AnimatedModel* pAnimatedModel = new AnimatedModel("animTest.dae", "AnimTest");
	//SkeletalAnimation* pWalkCycle = new SkeletalAnimation("WalkCycle", vec2(1, 10), 2, 0.1, 0);
    //pAnimatedModel->PlaySkeletalAnimation(pWalkCycle, true);

    //umEngine::DefaultRenderer->getObjectManager()->addObject(pAnimatedModel);

	Object3D* pTestOBJ = new Object3D(Mesh::generateCube(vec3(1,1,1)), "TestOBJ");
    pWorld3D->getObjectManager()->addObject(pTestOBJ);

	return pWorld3D;
}