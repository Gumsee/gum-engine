#include <Engine.h>
#include "../Examples.h"

World* Examples::createPhysicallyBasedRenderingExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(false);
	pWorld->getObjectManager()->getSkybox()->setTexture(GumEngine::Textures->getTexture("Sky/forest.hdr", true));


	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));

    Mesh *pSphereMesh = Object("Sphere.obj", "Sphere").getProperties()->pMesh;// Mesh::generateUVSphere(15, 15);

	Object *MetalPlateObj = new Object(pSphereMesh, "MetalPlate");
	MetalPlateObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("MetalPlate/diffuse.png"), 2);
	MetalPlateObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("MetalPlate/normal.png"), 14);
	MetalPlateObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("MetalPlate/specular.png"), 9);
	MetalPlateObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("MetalPlate/roughness.png"), 8);
	MetalPlateObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("MetalPlate/ao.png"), 7);
	MetalPlateObj->getMaterial()->setReflectivity(0);
	MetalPlateObj->getMaterial()->setTextureMultiplier(2);
	MetalPlateObj->setPosition(vec3(-5, 0, 0));
    pWorld->getObjectManager()->addObject(MetalPlateObj);

	Object *RustedIronObj = new Object(pSphereMesh, "RustedIron");
	RustedIronObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RustedIron/albedo.png"), 2);
	RustedIronObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RustedIron/normal.png"), 14);
	RustedIronObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RustedIron/metallic.png"), 9);
	RustedIronObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RustedIron/roughness.png"), 8);
	RustedIronObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RustedIron/ao.png"), 7);
	RustedIronObj->getMaterial()->setReflectivity(0);
	RustedIronObj->getMaterial()->setTextureMultiplier(1);
	RustedIronObj->setPosition(vec3(2.5, 0, 0));
    pWorld->getObjectManager()->addObject(RustedIronObj);
	
	Object *CobblestoneObj = new Object(pSphereMesh, "Cobblestone");
	CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/diffuse.png"), 2);
	CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/normal.png"), 14);
	CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/roughness.png"), 8);
	CobblestoneObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/ao.png"), 7);
	CobblestoneObj->getMaterial()->setReflectivity(0);
	CobblestoneObj->getMaterial()->setTextureMultiplier(1);
	CobblestoneObj->setPosition(vec3(-2.5, 0, 0));
    pWorld->getObjectManager()->addObject(CobblestoneObj);

		
	Object *GoldObj = new Object(pSphereMesh, "Gold");
	GoldObj->getMaterial()->setReflectivity(0.95);
	GoldObj->getMaterial()->setRoughness(0.01);
	GoldObj->getMaterial()->setColor(vec4(0.8, 0.67, 0, 1));
	GoldObj->setPosition(vec3(0, 0, 0));
    pWorld->getObjectManager()->addObject(GoldObj);

	Object *PlasticObj = new Object(pSphereMesh, "Plastic");
	PlasticObj->getMaterial()->setReflectivity(0);
	PlasticObj->getMaterial()->setRoughness(0.3);
	PlasticObj->getMaterial()->setSpecularity(0.4);
	PlasticObj->getMaterial()->setColor(vec4(1, 0, 0, 1));
	PlasticObj->setPosition(vec3(5, 0, 0));
    pWorld->getObjectManager()->addObject(PlasticObj);

	Object *GoldFoilObj = new Object(pSphereMesh, "GoldFoil");
	GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/albedo.png"), 2);
	GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/normal.png"), 14);
	GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/metallic.png"), 9);
	GoldFoilObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("GoldFoil/roughness.png"), 8);
	GoldFoilObj->getMaterial()->setReflectivity(0);
	GoldFoilObj->setPosition(vec3(-5, 2.5, 0));
    pWorld->getObjectManager()->addObject(GoldFoilObj);

	Object *LeatherObj = new Object(pSphereMesh, "Leather");
	LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/albedo.png"), 2);
	LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/normal.png"), 14);
	LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/roughness.png"), 8);
	LeatherObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Leather/ao.png"), 7);
	LeatherObj->getMaterial()->setReflectivity(0);
	LeatherObj->setPosition(vec3(-2.5, 2.5, 0));
    pWorld->getObjectManager()->addObject(LeatherObj);

	Object *RoughBronzeObj = new Object(pSphereMesh, "RoughBronze");
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/albedo.png"), 2);
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/normal.png"), 14);
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/roughness.png"), 8);
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/metallic.png"), 9);
	RoughBronzeObj->getMaterial()->setReflectivity(0);
	RoughBronzeObj->setPosition(vec3(2.5, 2.5, 0));
    pWorld->getObjectManager()->addObject(RoughBronzeObj);

	Object *PebblesObj = new Object(pSphereMesh, "Pebbles");
	PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/albedo.png"), 2);
	PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/normal.png"), 14);
	PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/roughness.png"), 8);
	PebblesObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pebbles/ao.png"), 7);
	PebblesObj->getMaterial()->setReflectivity(0);
	PebblesObj->setPosition(vec3(0, 2.5, 0));
    pWorld->getObjectManager()->addObject(PebblesObj);

	Object *PavementObj = new Object(pSphereMesh, "Pavement");
	PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/albedo.png"), 2);
	PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/normal.png"), 14);
	PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/roughness.png"), 8);
	PavementObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("Pavement/ao.png"), 7);
	PavementObj->getMaterial()->setReflectivity(0);
	PavementObj->setPosition(vec3(5, 2.5, 0));
    pWorld->getObjectManager()->addObject(PavementObj);

	return pWorld;
}
