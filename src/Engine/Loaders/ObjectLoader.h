#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <Essentials/Output.h>
#include <OpenGL/Mesh.h>
#include "../Object/Skeletal/SkeletalAnimation.h"
#include "../Object/Skeletal/Bone.h"

class ObjectLoader
{
public:
	std::vector<Mesh*> vMeshes;
    std::vector<Bone*> vBones;
    std::vector<SkeletalAnimation*> vSkeletalAnimations;
	Bone* rootBone;
	mat4 globalInverseTransform;

	ObjectLoader()  {};
	~ObjectLoader() {};

	void open(const std::string& fileName);
	void open(const aiScene *scene);
    const aiScene *getScene();
	Bone* FindBone(std::string name);

private:
	const aiScene *pScene;
    std::vector<aiNodeAnim*> ai_nodes_anim;

	void processNode(aiNode *node);
	Mesh* processMesh(aiMesh *mesh);
	void processAnimNode();
	void recursiveBuildSkeleton(aiNode* node, Bone *currentBone, int layer);
    void processBones(Mesh* mesh, aiMesh *pAiMesh);

    aiNode* FindAiNode(std::string name);
    aiNodeAnim* FindAiNodeAnim(std::string name);
};