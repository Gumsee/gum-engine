#pragma once
#include "Primitives/Mesh.h"
#include <gum-engine.h>
#include <GUI/Basics/Tabs.h>
#include <Engine/3D/Object/SceneObject.h>

World3D* createPostProcessingExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));


    Gum::File shaderballfile = ObjectManager::MODEL_ASSETS_PATH + "shaderball.obj";

	SceneObject *RoughBronzeObj = new SceneObject(shaderballfile, "RoughBronze");
    RoughBronzeObj->addInstance();
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/albedo.png"), 0);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/normal.png"), 14);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/roughness.png"), 8);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/metallic.png"), 9);
	RoughBronzeObj->getMaterial()->setReflectivity(0);
	RoughBronzeObj->getInstance()->setPosition(vec3(0, 0, 0));
    pWorld3D->getObjectManager()->addObject(RoughBronzeObj);

    SceneObject* floor = new SceneObject(Mesh::generatePlane(vec2(1,1)), "Floor");
    floor->addInstance();
	floor->getInstance()->setPosition(vec3(0, 0, 0));
    floor->getInstance()->setScale(vec3(10, 1, 10));
    pWorld3D->getObjectManager()->addObject(floor);

    //GaussianBlur* pGaussianBlur = new GaussianBlur(pWorld3D->getRenderCanvas(), Gum::Window->getRenderQuadSize(), 1);
    /*Brightfilter* pBrightfilter = new Brightfilter(GumEngine::DefaultRenderer->getRenderCanvas(), Gum::Window->getRenderQuadSize());
    GumEngine::DefaultRenderer->addPostProcessingEffect(pBrightfilter);*/

	return pWorld3D;
}