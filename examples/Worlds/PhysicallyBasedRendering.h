#pragma once
#include <gum-engine.h>
#include <Engine/3D/Object/SceneObject.h>

World3D* createPhysicallyBasedRenderingExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(false);
	pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/forest.hdr", true));


	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, -5),  vec3(600), "light1"));
	//pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	//pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	//pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));

    std::string shaderballfile = ObjectManager::MODEL_ASSETS_PATH + "shaderball.obj";

    float gapSize = 3.0f;

	SceneObject *MetalPlateObj = new SceneObject(shaderballfile, "MetalPlate");
    MetalPlateObj->addInstance();
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/diffuse.png"), 0);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/normal.png"), 14);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/specular.png"), 9);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/roughness.png"), 8);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/ao.png"), 7);
	MetalPlateObj->getMaterial()->setReflectivity(0);
	MetalPlateObj->getMaterial()->setTextureMultiplier(2);
	MetalPlateObj->getInstance()->setPosition(vec3(-gapSize * 2, 0, 0));
    pWorld3D->getObjectManager()->addObject(MetalPlateObj);

	SceneObject *RustedIronObj = new SceneObject(shaderballfile, "RustedIron");
    RustedIronObj->addInstance();
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/albedo.png"), 0);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/normal.png"), 14);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/metallic.png"), 9);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/roughness.png"), 8);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/ao.png"), 7);
	RustedIronObj->getMaterial()->setReflectivity(0);
	RustedIronObj->getMaterial()->setTextureMultiplier(1);
	RustedIronObj->getInstance()->setPosition(vec3(gapSize, 0, 0));
    pWorld3D->getObjectManager()->addObject(RustedIronObj);
	
	SceneObject *CobblestoneObj = new SceneObject(shaderballfile, "Cobblestone");
    CobblestoneObj->addInstance();
	CobblestoneObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Cobblestone/diffuse.png"), 0);
	CobblestoneObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Cobblestone/normal.png"), 14);
	CobblestoneObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Cobblestone/roughness.png"), 8);
	CobblestoneObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Cobblestone/ao.png"), 7);
	CobblestoneObj->getMaterial()->setReflectivity(0);
	CobblestoneObj->getMaterial()->setTextureMultiplier(1);
	CobblestoneObj->getInstance()->setPosition(vec3(-gapSize, 0, 0));
    pWorld3D->getObjectManager()->addObject(CobblestoneObj);

		
	SceneObject *GoldObj = new SceneObject(shaderballfile, "Gold");
    GoldObj->addInstance();
	GoldObj->getMaterial()->setReflectivity(0.95);
	GoldObj->getMaterial()->setRoughness(0.01);
	GoldObj->getMaterial()->setColor(rgba(204, 171, 0, 255));
	GoldObj->getInstance()->setPosition(vec3(0, 0, 0));
    pWorld3D->getObjectManager()->addObject(GoldObj);

	SceneObject *PlasticObj = new SceneObject(shaderballfile, "Plastic");
    PlasticObj->addInstance();
	PlasticObj->getMaterial()->setReflectivity(0);
	PlasticObj->getMaterial()->setRoughness(0.3);
	PlasticObj->getMaterial()->setSpecularity(0.4);
	PlasticObj->getMaterial()->setColor(rgba(255, 0, 0, 255));
	PlasticObj->getInstance()->setPosition(vec3(gapSize * 2, 0, 0));
    pWorld3D->getObjectManager()->addObject(PlasticObj);

	SceneObject *GoldFoilObj = new SceneObject(shaderballfile, "GoldFoil");
    GoldFoilObj->addInstance();
	GoldFoilObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("GoldFoil/albedo.png"), 0);
	GoldFoilObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("GoldFoil/normal.png"), 14);
	GoldFoilObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("GoldFoil/metallic.png"), 9);
	GoldFoilObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("GoldFoil/roughness.png"), 8);
	GoldFoilObj->getMaterial()->setReflectivity(0);
	GoldFoilObj->getInstance()->setPosition(vec3(-gapSize * 2, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(GoldFoilObj);

	SceneObject *LeatherObj = new SceneObject(shaderballfile, "Leather");
    LeatherObj->addInstance();
	LeatherObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Leather/albedo.png"), 0);
	LeatherObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Leather/normal.png"), 14);
	LeatherObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Leather/roughness.png"), 8);
	LeatherObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Leather/ao.png"), 7);
	LeatherObj->getMaterial()->setReflectivity(0);
	LeatherObj->getInstance()->setPosition(vec3(-gapSize, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(LeatherObj);

	SceneObject *RoughBronzeObj = new SceneObject(shaderballfile, "RoughBronze");
    RoughBronzeObj->addInstance();
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/albedo.png"), 0);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/normal.png"), 14);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/roughness.png"), 8);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RoughBronze/metallic.png"), 9);
	RoughBronzeObj->getMaterial()->setReflectivity(0);
	RoughBronzeObj->getInstance()->setPosition(vec3(gapSize, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(RoughBronzeObj);

	SceneObject *PebblesObj = new SceneObject(shaderballfile, "Pebbles");
    PebblesObj->addInstance();
	PebblesObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pebbles/albedo.png"), 0);
	PebblesObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pebbles/normal.png"), 14);
	PebblesObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pebbles/roughness.png"), 8);
	PebblesObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pebbles/ao.png"), 7);
	PebblesObj->getMaterial()->setReflectivity(0);
	PebblesObj->getInstance()->setPosition(vec3(0, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(PebblesObj);

	SceneObject *PavementObj = new SceneObject(shaderballfile, "Pavement");
    PavementObj->addInstance();
	PavementObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pavement/albedo.png"), 0);
	PavementObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pavement/normal.png"), 14);
	PavementObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pavement/roughness.png"), 8);
	PavementObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("Pavement/ao.png"), 7);
	PavementObj->getMaterial()->setReflectivity(0);
	PavementObj->getInstance()->setPosition(vec3(gapSize * 2, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(PavementObj);


    SceneObject* floor = new SceneObject(Mesh::generatePlane(vec2(1,1)), "Floor");
    floor->addInstance();
	floor->getInstance()->setPosition(vec3(0, 0, 0));
    floor->getInstance()->setScale(vec3(10, 1, 10));
    pWorld3D->getObjectManager()->addObject(floor);

	return pWorld3D;
}
