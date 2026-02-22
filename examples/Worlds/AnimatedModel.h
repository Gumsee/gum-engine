#pragma once
#include "Desktop/Window.h"
#include "System/Output.h"
#include <Engine/3D/Object/Skeletal/BoneRenderer/BoneRenderer.h>
#include <Engine/3D/Object/Skeletal/AnimatedModel.h>
#include <Engine/3D/Object/Skeletal/RagdollObjectInstance.h>
#include <gum-engine.h>

//WORKING
World3D* createAnimatedModelExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->renderSky(true);

	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, -5),  vec3(600), "light1"));

    //Default
    //AnimatedModel* pAnimatedModel = new AnimatedModel("/home/gumse/Downloads/dancing_vampire.dae", "AnimTest");
    AnimatedModel* pAnimatedModel = new AnimatedModel(ObjectManager::MODEL_ASSETS_PATH + "/testAnim.fbx", "AnimTest");
    RagdollObjectInstance* ragdollInstance = pAnimatedModel->addRagdollInstance(pWorld3D);

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

	SceneObject* floor = new SceneObject(Mesh::generateCube(vec3(90, 2, 90)), "Floor");
    floor->addPhysicsInstance(PhysicsObjectInstance::BOX, 0.0f, pWorld3D)->setPosition(vec3(0, -2, 0));
    pWorld3D->getObjectManager()->addObject(floor);

    //pWorld3D->addRenderable(new BoneRenderer(pAnimatedModel));

    pWorld3D->addUpdatable([ragdollInstance]() {
        if(Gum::Window::CurrentlyBoundWindow->getKeyboard()->checkKeyPressed(GUM_KEY_R))
            ragdollInstance->triggerRagdoll();
    });

	return pWorld3D;
}