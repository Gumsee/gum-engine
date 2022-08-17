#pragma once
#include <functional>
#include <vector>
#include <string>
#include <OpenGL/Mesh.h>
#include "../Object/Skeletal/SkeletalAnimation.h"
#include "../Object/Skeletal/Bone.h"

class ObjectLoader
{
public:
	ObjectLoader(const std::string& fileName, 
		std::function<void(Mesh* mesh)> nodefunc, 
		std::function<void(std::vector<Bone*> allbones, Bone* rootbone, std::vector<SkeletalAnimation*> skelanim, mat4 globalinverse)> skelfunc);
	~ObjectLoader();
};