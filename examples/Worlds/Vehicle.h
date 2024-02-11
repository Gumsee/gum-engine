#pragma once
#include <gum-engine.h>
#include <Engine/3D/Object/SceneObject.h>
#include <Engine/3D/Object/Vehicle/Vehicle.h>
#include <Engine/3D/Object/Terrain/Terrain.h>
#include <Engine/Texture/TextureManager.h>
#include <Engine/3D/World3D.h>
#include <Essentials/FPS.h>
#include <Graphics/Material.h>
#include <System/Output.h>

World3D* createVehicleExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true); //PREFILTER MAP RENDERN BEI GRADIANT
	//pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* light = new PointLight(vec3(10, 1, 0), vec3(30), "light");
    pWorld3D->getLightManager()->addPointLight(light);

    Terrain* terrain = new Terrain(vec2(10000, 10000), ivec2(1000, 1000), 200, 12345);

    SceneObject *terrainObj = new SceneObject(terrain, "terrain");
    terrainObj->addInstance();
    terrainObj->getInstance()->setPosition(vec3(-50, -5, -50));
    pWorld3D->getObjectManager()->addObject(terrainObj);
    

    Vehicle* car = new Vehicle(ObjectManager::MODEL_ASSETS_PATH + "/deluxecar.obj", "Car");
    car->addInstance(pWorld3D);
    pWorld3D->getObjectManager()->addObject(car);


    pWorld3D->addUpdatable([]() {
        //wall->getInstance()->increaseRotation(vec3(0, 20 * FPS::get(), 0));
    });


    return pWorld3D;
}