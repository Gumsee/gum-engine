#include <Engine.h>
#include "../Examples.h"

World* Examples::createBillboardsExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

    //Default
    Billboard* fixedBillboard = new Billboard(vec3(5,0,0));
    fixedBillboard->useFixedSize(true); 
    fixedBillboard->setPixelSize(vec2(200,200));
    fixedBillboard->setTexture(GumEngine::Textures->getTexture("Cobblestone/normal.png"));
    pWorld->addBillboard(fixedBillboard);

    Billboard* dynamicBillboard = new Billboard(vec3(-5,0,0));
    dynamicBillboard->useFixedSize(false);
    dynamicBillboard->setScale(vec2(1,1));
    dynamicBillboard->setTexture(GumEngine::Textures->getTexture("Cobblestone/normal.png"));
    pWorld->addBillboard(dynamicBillboard);

	return pWorld;
}
