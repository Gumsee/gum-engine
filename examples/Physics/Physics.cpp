#include <Engine.h>
#include "../Examples.h"
#include "Object/Object.h"

World* Examples::createPhysicsExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

    GumEngine::ActiveCamera->setZoomFactor(10);
    GumEngine::ActiveCamera->setOffset(50);

	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));


	Object* FloorObject = new Object(Mesh::generateCube(vec3(90, 2, 90)), "FloorObject");
    FloorObject->setPosition(vec3(0, -3, 0));
	FloorObject->addPhysics(BOX_SHAPE, false, 0.0f, FloorObject->getInstance(0));
    pWorld->getObjectManager()->addObject(FloorObject);

    int shape = BOX_SHAPE;
    int index = 0;
    int offset = 2;
    float gap = 2.0f;
    int dim = 5;
	Object* Cubes = new Object(Mesh::generateCube(vec3(1, 1, 1)), "Cubes");
	//Object* Cubes = new Object(Mesh::generateUVSphere(15, 15), "Cubes");
	Cubes->addPhysics(shape, true, 1.0f, Cubes->getInstance(0));
    Cubes->getMaterial()->setColor(vec4(1,1,1,1));
	Cubes->setPosition(vec3(-12.5f, offset, -12.5f), index);
	for(int i = 0; i < dim; i++)
	{
        for(int j = 0; j < dim; j++)
        {
            for(int k = 0; k < dim; k++)
            {
                Cubes->addInstance();
                index++;
                Cubes->setPosition(vec3(i * gap - dim * gap * 0.5f, j * gap + offset, k * gap - dim * gap * 0.5f), index);
                Cubes->addPhysics(shape, true, 1.0f, Cubes->getInstance(index));
            }
        }
    }

    for(int i = 0; i < dim; i++)
    {
        Cubes->addInstance();
        index++;
        Cubes->setPosition(vec3(i * gap, 300 +  i * gap, i * gap), index);
        Cubes->addPhysics(shape, true, 1.0f, Cubes->getInstance(index));
        //Cubes->setRotation(vec3(20, 52, 32), index);
    }

    pWorld->getObjectManager()->addObject(Cubes);

    //Object* camObj = new Object("FilmCamera.obj", "Camera");
    //camObj->setPosition(vec3(0,20,0));
    //camObj->writeObjectInfoToFile("testobj.txt");
    //pWorld->getObjectManager()->addObject(camObj);

	return pWorld;
}
