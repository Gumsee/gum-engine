#pragma once
#include "PhysicsObjectInstance.h"
#include "Graphics/Object3D.h"
#include "Graphics/Material.h"


class SceneObject : public Object3D
{
private:
    Material* pMaterial;

public:
    SceneObject(std::string name);
    SceneObject(Mesh *mesh, std::string name);
    SceneObject(std::string file, std::string name);
    ~SceneObject();

    void render();

    void setMaterial(Material* material);
    PhysicsObjectInstance* addPhysicsInstance(PhysicsObjectInstance::Shape shape, float mass, World3D* world, vec3 special = vec3(0));

    Material* getMaterial();
};