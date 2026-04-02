#pragma once
#include "../SceneObject.h"
#include <Graphics/Object3D.h>
#include <Graphics/Material.h>

class Vehicle : public SceneObject
{
private:

public:
	Vehicle(const Gum::File& file, const std::string& name);
	~Vehicle();

	void render();
    Object3DInstance* addInstance();


    // Getter
    void addWheel(Object3D wheelobj, vec3 offset);
};