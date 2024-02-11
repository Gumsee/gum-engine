#pragma once
#include "PhysicsObjectInstance.h"
#include "Graphics/Object3D.h"
#include "Graphics/Material.h"


class SceneObject : public Object3D
{
protected:
    Material* pMaterial;

public:
    SceneObject(std::string name);
    SceneObject(Mesh *mesh, std::string name);
    SceneObject(const Gum::File& file, const std::string& name);
    ~SceneObject();

    void render() override;

    void setMaterial(Material* material);
    PhysicsObjectInstance* addPhysicsInstance(PhysicsObjectInstance::Shape shape, float mass, World3D* world, vec3 special = vec3(0));

    Material* getMaterial();
};