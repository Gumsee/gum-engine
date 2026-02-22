#pragma once
#include <gum-engine.h>
#include <Engine/3D/Object/SceneObject.h>
#include <Engine/3D/World3D.h>
#include "../Examples.h"

World3D* createPhysicallyBasedRenderingExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->renderSky(false);
	pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/forest.hdr", true));


	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, -5),  vec3(600), "light1"));
	//pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	//pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	//pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));

    Gum::File shaderballfile = ObjectManager::MODEL_ASSETS_PATH + "shaderball.obj";

    float gapSize = 3.0f;

	SceneObject *MetalPlateObj = new SceneObject(shaderballfile, "MetalPlate");
    MetalPlateObj->addInstance();
    MetalPlateObj->setMaterial(new Material(Examples::assetPath + "materials/metalplate.gummtl"));
	MetalPlateObj->getMaterial()->setTextureMultiplier(2);
	MetalPlateObj->getInstance()->setPosition(vec3(-gapSize * 2, 0, 0));
    pWorld3D->getObjectManager()->addObject(MetalPlateObj);

	SceneObject *RustedIronObj = new SceneObject(shaderballfile, "RustedIron");
    RustedIronObj->addInstance();
    RustedIronObj->setMaterial(new Material(Examples::assetPath + "materials/rustediron.gummtl"));
	RustedIronObj->getInstance()->setPosition(vec3(gapSize, 0, 0));
    pWorld3D->getObjectManager()->addObject(RustedIronObj);
	
	SceneObject *CobblestoneObj = new SceneObject(shaderballfile, "Cobblestone");
    CobblestoneObj->addInstance();
    CobblestoneObj->setMaterial(new Material(Examples::assetPath + "materials/cobblestone.gummtl"));
	CobblestoneObj->getInstance()->setPosition(vec3(-gapSize, 0, 0));
    pWorld3D->getObjectManager()->addObject(CobblestoneObj);

		
	SceneObject *GoldObj = new SceneObject(shaderballfile, "Gold");
    GoldObj->addInstance();
	GoldObj->getMaterial()->setReflectivity(0.5);
	GoldObj->getMaterial()->setRoughness(0.01);
	GoldObj->getMaterial()->setColor(rgba(204, 171, 0, 255));
	GoldObj->getInstance()->setPosition(vec3(0, 0, 0));
    pWorld3D->getObjectManager()->addObject(GoldObj);

	SceneObject *PlasticObj = new SceneObject(shaderballfile, "Plastic");
    PlasticObj->addInstance();
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
    LeatherObj->setMaterial(new Material(Examples::assetPath + "materials/leather.gummtl"));
	LeatherObj->getInstance()->setPosition(vec3(-gapSize, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(LeatherObj);

	SceneObject *RoughBronzeObj = new SceneObject(shaderballfile, "RoughBronze");
    RoughBronzeObj->addInstance();
    RoughBronzeObj->setMaterial(new Material(Examples::assetPath + "materials/roughbronze.gummtl"));
	RoughBronzeObj->getInstance()->setPosition(vec3(gapSize, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(RoughBronzeObj);

	SceneObject *PebblesObj = new SceneObject(shaderballfile, "Pebbles");
    PebblesObj->addInstance();
    PebblesObj->setMaterial(new Material(Examples::assetPath + "materials/pebbles.gummtl"));
	PebblesObj->getInstance()->setPosition(vec3(0, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(PebblesObj);

	SceneObject *PavementObj = new SceneObject(shaderballfile, "Pavement");
    PavementObj->addInstance();
    PavementObj->setMaterial(new Material(Examples::assetPath + "materials/pavement.gummtl"));
	PavementObj->getInstance()->setPosition(vec3(gapSize * 2, gapSize, 0));
    pWorld3D->getObjectManager()->addObject(PavementObj);


    SceneObject* floor = new SceneObject(Mesh::generatePlane(vec2(1,1)), "Floor");
    floor->addInstance();
    floor->setMaterial(new Material(Examples::assetPath + "materials/pebbles.gummtl"));
    floor->getMaterial()->setTextureMultiplier(10);
	floor->getInstance()->setPosition(vec3(0, 0, 0));
    floor->getInstance()->setScale(vec3(20, 1, 20));
    pWorld3D->getObjectManager()->addObject(floor);

	return pWorld3D;
}
