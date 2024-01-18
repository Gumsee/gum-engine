#pragma once
#include "System/Output.h"
#include <gum-engine.h>
#include <Engine/3D/Object/Skeletal/BoneRenderer/BoneRenderer.h>
#include <Engine/3D/Object/Skeletal/AnimatedModel.h>

//WORKING
World3D* createAnimatedModelExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, -5),  vec3(600), "light1"));

    //Default
    //AnimatedModel* pAnimatedModel = new AnimatedModel("/home/gumse/Downloads/dancing_vampire.dae", "AnimTest");
    AnimatedModel* pAnimatedModel = new AnimatedModel(ObjectManager::MODEL_ASSETS_PATH + "/testAnim.fbx", "AnimTest");
    pAnimatedModel->addInstance();

	SkeletalAnimation* pWalkCycle = pAnimatedModel->getSkeleton()->getAnimation(0);
    pWalkCycle->activate(true);
    pWalkCycle->setSpeed(20);
    Gum::Output::print("Playing " + pWalkCycle->getName());

	SkeletalAnimation* pWavingCycle = pAnimatedModel->getSkeleton()->getAnimation(1);
    pWavingCycle->activate(true);
    pWavingCycle->setSpeed(10);
    pWavingCycle->removeUnusedKeyframes();
    Gum::Output::print("Playing " + pWavingCycle->getName());

    pWorld3D->getObjectManager()->addObject(pAnimatedModel);

    SceneObject* floor = new SceneObject(Mesh::generatePlane(vec2(1,1)), "Floor");
    floor->addInstance();
	floor->getInstance()->setPosition(vec3(0, 0, 0));
    floor->getInstance()->setScale(vec3(10, 1, 10));
    pWorld3D->getObjectManager()->addObject(floor);

    //pWorld3D->addRenderable(new BoneRenderer(pAnimatedModel));

	return pWorld3D;
}