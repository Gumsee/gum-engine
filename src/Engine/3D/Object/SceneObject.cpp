#include "SceneObject.h"
#include "../World3D.h"
#include "../../Material/MaterialManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ShaderProgram.h"
#include "System/MemoryManagement.h"

SceneObject::SceneObject(Mesh *mesh, std::string name) : Object3D(mesh, name)
{
    pMaterial = new Material(*Gum::MaterialManager::getDefaultMaterial());
}

SceneObject::SceneObject(std::string name) 
{
    pMaterial = new Material(*Gum::MaterialManager::getDefaultMaterial());
}

SceneObject::SceneObject(std::string file, std::string name) : Object3D(file, name)
{
    pMaterial = new Material(*Gum::MaterialManager::getDefaultMaterial());
}

SceneObject::~SceneObject()
{

}

void SceneObject::render()
{
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("color", pMaterial->getColor().getGLColor());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("reflectivity", pMaterial->getReflectivity());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("refractivity", pMaterial->getRefractivity());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("specularity", pMaterial->getSpecularity());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("roughness", pMaterial->getRoughness());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("TextureMultiplier", pMaterial->getTextureMultiplier());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasTexture", pMaterial->hasTexture());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasNormalMap", pMaterial->hasNormalMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasSpecularMap", pMaterial->hasSpecularMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasReflectionMap", pMaterial->hasReflectionMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasRefractionMap", pMaterial->hasRefractionMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasRoughnessMap", pMaterial->hasRoughnessMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasAmbientOcclusionMap", pMaterial->hasAmbientOcclusionMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasDisplacementMap", pMaterial->hasDisplacementMap());
    ShaderProgram::getCurrentlyBoundShader()->loadUniform("viewPos", Camera::getActiveCamera()->getPosition());

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