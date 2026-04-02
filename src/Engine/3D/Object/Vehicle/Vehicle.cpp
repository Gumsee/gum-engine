#include "Vehicle.h"


Vehicle::Vehicle(const Gum::File& file, const std::string& name)
    : SceneObject(file, name)
{

}

Vehicle::~Vehicle()
{

}


void Vehicle::render()
{
    //Render body
    SceneObject::render();

    //Render wheels
}

Object3DInstance* Vehicle::addInstance()
{
    Object3DInstance* instance = new Object3DInstance(this);
    SceneObject::addInstance(instance);

    return instance;
}


// Getter
void Vehicle::addWheel(Object3D wheelobj, vec3 offset)
{
    (void)wheelobj;
    (void)offset;
}