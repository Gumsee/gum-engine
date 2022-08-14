#include <Engine.h>
#include "../Examples.h"

World* Examples::createIrradiancePBRExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(false);
	pWorld->getObjectManager()->getSkybox()->setTexture(GumEngine::Textures->getTexture("Sky/spaichingen_hill_8k.hdr", true));


	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 10),  vec3(300), "light1"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 10),   vec3(300), "light2"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 10), vec3(300), "light3"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 10),  vec3(300), "light4"));

    Mesh *pSphereMesh = Mesh::generateUVSphere(32, 32);

    int dim = 10;
    float gap = 2.5f;
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            Object *currobj = new Object(pSphereMesh, "IrradianceObj" + std::to_string(i) + "_" + std::to_string(j));
            currobj->setPosition(vec3(
                i * gap - dim * 0.5f * gap + gap * 0.5f, 
                j * gap - dim * 0.5f * gap + gap * 0.5f, 
                0
            ), 0);
            currobj->getMaterial()->setRoughness((float)i / (float)dim);
            currobj->getMaterial()->setSpecularity((float)j / (float)dim);
            currobj->getMaterial()->setColor(vec4(1, 0.0, 0.0, 1));
            pWorld->getObjectManager()->addObject(currobj);   
        }
    }
	
	return pWorld;
}