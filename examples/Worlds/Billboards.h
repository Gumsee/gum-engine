#pragma once
#include "System/Output.h"
#include <Engine/Texture/TextureManager.h>
#include <gum-engine.h>

World3D* createBillboardsExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

    //Default
    Billboard* fixedBillboard = new Billboard(vec3(5,0,0));
    fixedBillboard->useFixedSize(true); 
    fixedBillboard->setPixelSize(vec2(200,200));
    fixedBillboard->setTexture(Gum::TextureManager::getTexture("boii.jpg", true));
    //Texture2D* tex = (Texture2D*)(Gum::TextureManager::getTexture("boii.jpg"));
    pWorld3D->addBillboard(fixedBillboard);

    Billboard* dynamicBillboard = new Billboard(vec3(-5,0,0));
    dynamicBillboard->useFixedSize(false);
    dynamicBillboard->setScale(vec2(1,1));
    dynamicBillboard->setTexture(Gum::TextureManager::getTexture("boii.jpg"));
    pWorld3D->addBillboard(dynamicBillboard);

	return pWorld3D;
}
