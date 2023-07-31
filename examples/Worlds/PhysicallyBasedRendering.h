#pragma once
#include <Engine/3D/Object/SceneObject.h>
#include <Engine/Texture/TextureManager.h>
#include <gum-engine.h>
#include <OpenGL/Object3D.h>

World3D* createPhysicallyBasedRenderingExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(false);
	pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/forest.hdr", true));


	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));

    //Mesh *pSphereMesh = Object3D("Sphere.obj", "Sphere").getMesh();
    Mesh *pSphereMesh = Mesh::generateUVSphere(15, 15);

	SceneObject *MetalPlateObj = new SceneObject(pSphereMesh, "MetalPlate");
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/diffuse.png"), 2);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/normal.png"), 14);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/specular.png"), 9);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/roughness.png"), 8);
	MetalPlateObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("MetalPlate/ao.png"), 7);
	MetalPlateObj->getMaterial()->setReflectivity(0);
	MetalPlateObj->getMaterial()->setTextureMultiplier(2);
	MetalPlateObj->getInstance()->setPosition(vec3(-5, 0, 0));
    pWorld3D->getObjectManager()->addObject(MetalPlateObj);

	SceneObject *RustedIronObj = new SceneObject(pSphereMesh, "RustedIron");
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/albedo.png"), 2);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/normal.png"), 14);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/metallic.png"), 9);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/roughness.png"), 8);
	RustedIronObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/ao.png"), 7);
	RustedIronObj->getMaterial()->setReflectivity(0);
	RustedIronObj->getMaterial()->setTextureMultiplier(1);
	RustedIronObj->getInstance()->setPosition(vec3(2.5, 0, 0));
    pWorld3D->getObjectManager()->addObject(RustedIronObj);
	
	Object3D *CobblestoneObj = new Object3D(pSphereMesh, "Cobblestone");
	//CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/diffuse.png"), 2);
	//CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/normal.png"), 14);
	//CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/roughness.png"), 8);
	//CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/ao.png"), 7);
	//CobblestoneObj->getMaterial()->setReflectivity(0);
	//CobblestoneObj->getMaterial()->setTextureMultiplier(1);
	CobblestoneObj->getInstance()->setPosition(vec3(-2.5, 0, 0));
    pWorld3D->getObjectManager()->addObject(CobblestoneObj);

		
	Object3D *GoldObj = new Object3D(pSphereMesh, "Gold");
	//GoldObj->getMaterial()->setReflectivity(0.95);
	//GoldObj->getMaterial()->setRoughness(0.01);
	//GoldObj->getMaterial()->setColor(vec4(0.8, 0.67, 0, 1));
	GoldObj->getInstance()->setPosition(vec3(0, 0, 0));
    pWorld3D->getObjectManager()->addObject(GoldObj);

	Object3D *PlasticObj = new Object3D(pSphereMesh, "Plastic");
	//PlasticObj->getMaterial()->setReflectivity(0);
	//PlasticObj->getMaterial()->setRoughness(0.3);
	//PlasticObj->getMaterial()->setSpecularity(0.4);
	//PlasticObj->getMaterial()->setColor(vec4(1, 0, 0, 1));
	PlasticObj->getInstance()->setPosition(vec3(5, 0, 0));
    pWorld3D->getObjectManager()->addObject(PlasticObj);

	Object3D *GoldFoilObj = new Object3D(pSphereMesh, "GoldFoil");
	//GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/albedo.png"), 2);
	//GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/normal.png"), 14);
	//GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/metallic.png"), 9);
	//GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/roughness.png"), 8);
	//GoldFoilObj->getMaterial()->setReflectivity(0);
	GoldFoilObj->getInstance()->setPosition(vec3(-5, 2.5, 0));
    pWorld3D->getObjectManager()->addObject(GoldFoilObj);

	Object3D *LeatherObj = new Object3D(pSphereMesh, "Leather");
	//LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/albedo.png"), 2);
	//LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/normal.png"), 14);
	//LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/roughness.png"), 8);
	//LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/ao.png"), 7);
	//LeatherObj->getMaterial()->setReflectivity(0);
	LeatherObj->getInstance()->setPosition(vec3(-2.5, 2.5, 0));
    pWorld3D->getObjectManager()->addObject(LeatherObj);

	Object3D *RoughBronzeObj = new Object3D(pSphereMesh, "RoughBronze");
	//RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/albedo.png"), 2);
	//RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/normal.png"), 14);
	//RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/roughness.png"), 8);
	//RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/metallic.png"), 9);
	//RoughBronzeObj->getMaterial()->setReflectivity(0);
	RoughBronzeObj->getInstance()->setPosition(vec3(2.5, 2.5, 0));
    pWorld3D->getObjectManager()->addObject(RoughBronzeObj);

	Object3D *PebblesObj = new Object3D(pSphereMesh, "Pebbles");
	//PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/albedo.png"), 2);
	//PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/normal.png"), 14);
	//PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/roughness.png"), 8);
	//PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/ao.png"), 7);
	//PebblesObj->getMaterial()->setReflectivity(0);
	PebblesObj->getInstance()->setPosition(vec3(0, 2.5, 0));
    pWorld3D->getObjectManager()->addObject(PebblesObj);

	Object3D *PavementObj = new Object3D(pSphereMesh, "Pavement");
	//PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/albedo.png"), 2);
	//PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/normal.png"), 14);
	//PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/roughness.png"), 8);
	//PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/ao.png"), 7);
	//PavementObj->getMaterial()->setReflectivity(0);
	PavementObj->getInstance()->setPosition(vec3(5, 2.5, 0));
    pWorld3D->getObjectManager()->addObject(PavementObj);

	return pWorld3D;
}
