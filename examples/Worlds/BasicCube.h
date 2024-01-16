#pragma once
#include "Engine/3D/Object/SceneObject.h"
#include "Engine/Texture/TextureManager.h"
#include "Essentials/FPS.h"
#include "Graphics/Material.h"
#include "System/Output.h"
#include <gum-engine.h>

World3D* createBasicCubeExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true); //PREFILTER MAP RENDERN BEI GRADIANT
	//pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* light = new PointLight(vec3(10, 1, 0), vec3(30), "light");
    pWorld3D->getLightManager()->addPointLight(light);


    /*SceneObject *cube = new SceneObject(Mesh::generateCube(vec3(2)), "cube");
    cube->addInstance();
    //cube->addPhysicsInstance(PhysicsObjectInstance::BOX, 1.0f, pWorld3D);
	cube->getMaterial()->setColor(rgba(255,255,255,255));
	cube->getMaterial()->setRoughness(0.2f);
	cube->getMaterial()->setSpecularity(0.5f);
    cube->getMaterial()->setTexture(Gum::TextureManager::getTexture("normaltest.jpg"), GUM_MATERIAL_NORMAL_MAP);
	cube->getInstance()->setPosition(vec3(0, 0, 0));
    //RoughBronzeObj->getInstance()->renderOutline = true;
    pWorld3D->getObjectManager()->addObject(cube);*/

    SceneObject *wall = new SceneObject(Mesh::generatePlane(vec2(2)), "wall");
    wall->addInstance();
    //cube->addPhysicsInstance(PhysicsObjectInstance::BOX, 1.0f, pWorld3D);
	wall->getMaterial()->setColor(rgba(255,255,255,255));
	wall->getMaterial()->setRoughness(0.2f);
	wall->getMaterial()->setSpecularity(0.5f);
    wall->getMaterial()->setTexture(Gum::TextureManager::getTexture("TestBricks/albe.jpg"), GUM_MATERIAL_TEXTURE0);
    wall->getMaterial()->setTexture(Gum::TextureManager::getTexture("TestBricks/norm.jpg"), GUM_MATERIAL_NORMAL_MAP);
    wall->getMaterial()->setTexture(Gum::TextureManager::getTexture("TestBricks/disp.jpg"), GUM_MATERIAL_DISPLACEMENT_MAP);
	wall->getInstance()->setPosition(vec3(10, 0, 5));
	wall->getInstance()->setRotation(vec3(-90, 0, 0));
    //RoughBronzeObj->getInstance()->renderOutline = true;
    pWorld3D->getObjectManager()->addObject(wall);

    pWorld3D->addUpdateable([wall]() {
        //wall->getInstance()->increaseRotation(vec3(0, 20 * FPS::get(), 0));
    });


    return pWorld3D;
}