#include "SceneObject.h"
#include "../World3D.h"
#include "../../Material/MaterialManager.h"
#include "OpenGL/ShaderProgram.h"
#include "System/MemoryManagement.h"

SceneObject::SceneObject(Mesh *mesh, std::string name) : Object3D(mesh, name)
{
    pMaterial = new Material(*Gum::MaterialManager::getDefaultMaterial());
}

SceneObject::SceneObject(std::string name) 
{
    pMaterial = new Material(*Gum::MaterialManager::getDefaultMaterial());
}

SceneObject::~SceneObject()
{

}

void SceneObject::render()
{
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("color", pMaterial->getColor());
    pMaterial->bindTextures();
    renderMesh();
    pMaterial->unbindTextures();
}


PhysicsObjectInstance* SceneObject::addPhysicsInstance(PhysicsObjectInstance::Shape shape, float mass, World3D* world, vec3 special)
{
    PhysicsObjectInstance* instance = new PhysicsObjectInstance(this, shape, mass, world, special);
    addInstance(instance);

    return instance;
}


//
// Setter
//
void SceneObject::setMaterial(Material* material) 
{ 
    Gum::_delete(pMaterial);
    this->pMaterial = new Material(*material);
}


//
// Getter
//
Material* SceneObject::getMaterial()               { return this->pMaterial; }