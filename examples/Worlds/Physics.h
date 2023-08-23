#pragma once
#include "Engine/3D/Object/PhysicsObjectInstance.h"
#include "Engine/3D/Object/SceneObject.h"
#include <gum-engine.h>
#include <Graphics/Object3D.h>

World3D* createPhysicsExample()
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

    Camera::getActiveCamera()->setZoomFactor(10);
    ((Camera3D*)Camera::getActiveCamera())->setOffset(50);

	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));


	SceneObject* FloorObject = new SceneObject(Mesh::generateCube(vec3(90, 2, 90)), "FloorObject");
    FloorObject->addPhysicsInstance(PhysicsObjectInstance::BOX, 0.0f, pWorld3D)->setPosition(vec3(0, -3, 0));
	//FloorObject->getInstance()->addPhysics(BOX_SHAPE, false, 0.0f, FloorObject->getInstance(0));
    pWorld3D->getObjectManager()->addObject(FloorObject);

    PhysicsObjectInstance::Shape shape = PhysicsObjectInstance::BOX;
    int offset = 2;
    float gap = 4.0f;
    int dim = 5;
	//SceneObject* Cubes = new SceneObject(Mesh::generateCube(vec3(1, 1, 1)), "Cubes");
	SceneObject* Cubes = new SceneObject(Mesh::generateUVSphere(15, 15), "Cubes");

    Cubes->getMaterial()->setColor(vec4(1,0,1,1));
	for(int i = 0; i < dim; i++)
     for(int j = 0; j < dim; j++)
      for(int k = 0; k < dim; k++)
      {
        Cubes->addPhysicsInstance(shape, 1.0f, pWorld3D)->setPosition(vec3(i * gap - dim * gap * 0.5f, j * gap + offset, k * gap - dim * gap * 0.5f));
      }

    for(int i = 0; i < dim; i++)
    {
        PhysicsObjectInstance* instance = Cubes->addPhysicsInstance(shape, 1.0f, pWorld3D);
        instance->setPosition(vec3(i * gap * 0.5f, 300 +  i * gap * 0.5f, i * gap * 0.5f));
        instance->setRotation(vec3(20, 52, 32));
    }

    pWorld3D->getObjectManager()->addObject(Cubes);

    //Object* camObj = new Object("FilmCamera.obj", "Camera");
    //camObj->setPosition(vec3(0,20,0));
    //camObj->writeObjectInfoToFile("testobj.txt");
    //pWorld3D->getObjectManager()->addObject(camObj);

	return pWorld3D;
}
