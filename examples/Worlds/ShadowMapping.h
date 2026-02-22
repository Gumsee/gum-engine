#pragma once
#include <gum-engine.h>
#include <Engine/3D/Object/SceneObject.h>
#include <Engine/3D/Object/Terrain/Terrain.h>
#include <Engine/Texture/TextureManager.h>
#include <Engine/3D/World3D.h>
#include <Engine/3D/Renderer3D.h>
#include <Essentials/Time.h>
#include <Graphics/Material.h>
#include <System/Output.h>
#include "../Examples.h"
#include "Essentials/SerializationData.h"

World3D* createShadowMappingExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->renderSky(false); //PREFILTER MAP RENDERN BEI GRADIANT
	pWorld3D->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/aerodynamics_workshop_8k.hdr", true));



    Terrain* terrain = new Terrain(vec2(1000, 1000), ivec2(100, 100), 0, 12345);
    Mesh* terrain2 = Mesh::generatePlane(vec2(1000, 1000));

    SceneObject *terrainObj = new SceneObject(terrain2, "terrain");
    terrainObj->addInstance();
    terrainObj->getInstance()->setPosition(vec3(-terrain->getSize().x * 0.5f, -5, -terrain->getSize().y * 0.5f));
    //terrainObj->setMaterial(new Material(Examples::assetPath + "/materials/aerialgrass.gummtl"));
    terrainObj->getMaterial()->setTextureMultiplier(10);
    pWorld3D->getObjectManager()->addObject(terrainObj);


    Mesh *pBoxMesh = Mesh::generateCube(vec3(1,1,1));
	SceneObject* pCubeObject = new SceneObject(pBoxMesh, "Cube");
    pCubeObject->getMaterial()->setColor(rgba(255, 0, 100, 255));
    pCubeObject->getMaterial()->setRoughness(0.8f);
    pCubeObject->getMaterial()->setSpecularity(0.5f);

	for(int i = 0; i < 200; i++)
	{
		Object3DInstance* instance = pCubeObject->addInstance();
		vec2 pos(
			Gum::Maths::randf(-terrain->getSize().x * 0.5f, terrain->getSize().x * 0.5f), 
			Gum::Maths::randf(-terrain->getSize().y * 0.5f, terrain->getSize().y * 0.5f)
		);
		float scale = Gum::Maths::randf(1, 10);
		instance->setPosition(vec3(
			pos.x,
			terrain->getHeight(pos.x+terrain->getSize().x * 0.5f, pos.y+terrain->getSize().y * 0.5f) + scale * 0.5f,
			pos.y
		));
		instance->setScale(scale);
        instance->setRotation(vec3(Gum::Maths::randf(0, 360),Gum::Maths::randf(0, 360),Gum::Maths::randf(0, 360)));
	}

	for(int i = 0; i < 100; i++)
	{
        vec2 lightpos(
			Gum::Maths::randf(-terrain->getSize().x * 0.5f, terrain->getSize().x * 0.5f), 
			Gum::Maths::randf(-terrain->getSize().y * 0.5f, terrain->getSize().y * 0.5f)
		);
        PointLight* light = new PointLight(vec3(
			lightpos.x,
			terrain->getHeight(lightpos.x+terrain->getSize().x * 0.5f, lightpos.y+terrain->getSize().y * 0.5f) + 0.1f,
			lightpos.y
		), vec3::random(vec3(2.f), vec3(5000.0f)), "light");
        pWorld3D->getLightManager()->addPointLight(light);
    }

    pWorld3D->getObjectManager()->addObject(pCubeObject);
	
	return pWorld3D;
}
