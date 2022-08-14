#include <Engine.h>
#include <Maths/vec.h>
#include <Maths/mat.h>
#include <Object/Skeletal/AnimatedModel.h>
#include "../Examples.h"

World* Examples::createAnimatedModelExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

    //Default
    //AnimatedModel* pAnimatedModel = new AnimatedModel("animTest.dae", "AnimTest");
	//SkeletalAnimation* pWalkCycle = new SkeletalAnimation("WalkCycle", vec2(1, 10), 2, 0.1, 0);
    //pAnimatedModel->PlaySkeletalAnimation(pWalkCycle, true);

    //umEngine::DefaultRenderer->getObjectManager()->addObject(pAnimatedModel);

	Object* pTestOBJ = new Object(Mesh::generateCube(vec3(1,1,1)), "TestOBJ");
    pWorld->getObjectManager()->addObject(pTestOBJ);

	return pWorld;
}