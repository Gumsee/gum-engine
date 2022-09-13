#include "ObjectLoader.h"
#include <System/MemoryManagement.h>
#include <Essentials/Tools.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <System/IO/Output.h>


std::vector<aiNodeAnim*> ai_nodes_anim;
std::vector<Mesh*> vMeshes;
std::vector<Bone*> vBones;
std::vector<SkeletalAnimation*> vSkeletalAnimations;

void processNode(aiNode *node, const aiScene *scene, std::function<void(Mesh* mesh)> nodefunc);
Mesh* processMesh(aiMesh *mesh);
void processAnimNode(const aiScene *scene);
void recursiveBuildSkeleton(aiNode* node, Bone *currentBone, int layer);
void processBones(Mesh* mesh, aiMesh *pAiMesh);

aiNode* FindAiNode(std::string name);
aiNodeAnim* FindAiNodeAnim(std::string name);
Bone* FindBone(std::string name);

ObjectLoader::ObjectLoader(const std::string& fileName, 
		std::function<void(Mesh* mesh)> nodefunc, 
		std::function<void(std::vector<Bone*> allbones, Bone* rootbone, std::vector<SkeletalAnimation*> skelanim, mat4 globalinverse)> skelfunc)
{
    Assimp::Importer importer;
	const aiScene *pScene = importer.ReadFile(fileName, aiProcess_RemoveRedundantMaterials | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices);
    // check for errors
    if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) // if is Not Zero
    {
        Gum::Output::fatal("ObjectLoader: " + fileName + " ERROR::ASSIMP:: " + importer.GetErrorString());
        return;
    }
    processAnimNode(pScene);
    processNode(pScene->mRootNode, pScene, nodefunc);

    if(skelfunc != nullptr)
    {
        
        Bone* rootBone = new Bone(vMeshes[0], 0, "root", mat4(), mat4());
        recursiveBuildSkeleton(pScene->mRootNode, rootBone, 0);
        mat4 globalInverseTransform = mat4::inverse(Tools::aiMatrix4x4ToMat4(pScene->mRootNode->mTransformation));
        skelfunc(vBones, rootBone, vSkeletalAnimations, globalInverseTransform);
    }
    else 
    {
        for(SkeletalAnimation *anim : vSkeletalAnimations)
            Gum::_delete(anim);

        for(Bone *bone : vBones)
            Gum::_delete(bone);
    }
    
    Gum::Output::info("Loaded Object: " + fileName + ", numMeshes: " + std::to_string(vMeshes.size()) + ", Children: " + std::to_string(pScene->mRootNode->mNumChildren));

    importer.FreeScene();
}

ObjectLoader::~ObjectLoader() 
{

}

//We only get data from the first mSkeletalAnimation because 
//Assimp crushes all of the SkeletalAnimation data into one
//large sequence of data known as mSkeletalAnimation.
//Assimp does not support multiple mSkeletalAnimations, surprisingly.
void processAnimNode(const aiScene *scene)
{
    if(scene->mNumAnimations != 0)
        for(unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
            ai_nodes_anim.push_back(scene->mAnimations[0]->mChannels[i]);
}

void recursiveBuildSkeleton(aiNode* node, Bone *currentBone, int layer)
{
    if(currentBone == nullptr)
        return;

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        Bone *childBone = FindBone(node->mChildren[i]->mName.data);
        currentBone->addChild(childBone);
        childBone->setParent(currentBone);

        recursiveBuildSkeleton(node->mChildren[i], childBone, layer + 1);
    }
}

void processNode(aiNode *node, const aiScene *scene, std::function<void(Mesh* mesh)> nodefunc)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh *retMesh = processMesh(mesh);
        processBones(retMesh, mesh);

        retMesh->offsetMatrix = Tools::aiMatrix4x4ToMat4(scene->mRootNode->mTransformation) * Tools::aiMatrix4x4ToMat4(node->mTransformation);
        nodefunc(retMesh);
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, nodefunc);
}


void processBones(Mesh* mesh, aiMesh *pAiMesh)
{
    for (uint i = 0 ; i < pAiMesh->mNumBones ; i++) 
    {
        std::string BoneName = pAiMesh->mBones[i]->mName.data;
        int BoneIndex = 0;
        if (FindBone(BoneName) == nullptr)  { BoneIndex = i; }
        else                                { BoneIndex = FindBone(BoneName)->getID(); }

        mat4 BoneOffset = Tools::aiMatrix4x4ToMat4(pAiMesh->mBones[BoneIndex]->mOffsetMatrix);
        Bone* bone = new Bone(mesh, BoneIndex, BoneName, BoneOffset, BoneOffset);
        bone->setNodeAnim(FindAiNodeAnim(BoneName));


        for (uint j = 0 ; j < pAiMesh->mBones[BoneIndex]->mNumWeights ; j++) 
        {
            uint VertexID = pAiMesh->mBones[BoneIndex]->mWeights[j].mVertexId;
            float Weight = pAiMesh->mBones[BoneIndex]->mWeights[j].mWeight;
            if(mesh->getVertex(VertexID).Weights.x == 0)
            {
                mesh->getVertexPtr(VertexID)->Weights.x = Weight;
                mesh->getVertexPtr(VertexID)->JointIDs.x = BoneIndex;
            }
            else
            {
                if(mesh->getVertex(VertexID).Weights.y == 0)
                {
                    mesh->getVertexPtr(VertexID)->Weights.y = Weight;
                    mesh->getVertexPtr(VertexID)->JointIDs.y = BoneIndex;
                }
                else
                {
                    if(mesh->getVertex(VertexID).Weights.z == 0)
                    {
                        mesh->getVertexPtr(VertexID)->Weights.z = Weight;
                        mesh->getVertexPtr(VertexID)->JointIDs.z = BoneIndex;
                    }
                }
            }
        }
        vBones.push_back(bone);
    }
}

Mesh* processMesh(aiMesh *mesh)
{
    Mesh *pMesh = new Mesh();
    pMesh->name = mesh->mName.C_Str();
    pMesh->iMatIndex = mesh->mMaterialIndex;

    // Walk through each of the mesh's positions
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex currentVertex;
        vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        currentVertex.position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        currentVertex.normal = vector;
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            currentVertex.textureCoord = vec;
        }
        else
            currentVertex.textureCoord = vec2(0,0);
        
        // tangent
        if(mesh->HasTangentsAndBitangents())
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            currentVertex.tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            currentVertex.bitangent = vector;
        }
        else
        {
            currentVertex.tangent = vec3(0,1,0);
        }

        pMesh->addVertex(currentVertex);
    }

    // now go through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            pMesh->addIndex(face.mIndices[j]);
        }
    }
    
    //If weights don't add up to 1, add the rest to the first bone
    for(unsigned int i = 0; i < pMesh->numVertices(); i++)
    {
        float allWeights = pMesh->getVertex(i).Weights.x + pMesh->getVertex(i).Weights.y + pMesh->getVertex(i).Weights.z;
        if(allWeights < 1)
        {
            float missing = (1 - allWeights);
            pMesh->getVertexPtr(i)->Weights.x += missing;
        }
    }

    vMeshes.push_back(pMesh);
    return pMesh;
}


//This function simply scans our vector bones and checks if
//any name matches the name we're looking for, if it doesn't
//find any, we return nullptr.
//Keep in mind, the bones vector is empty at the point of writing this,
//but when this function is called it will already be filled up.
Bone* FindBone(std::string name)
{
    for(size_t i = 0; i < vBones.size(); i++)
        if(vBones[i]->getName() == name)
            return vBones[i];
    return nullptr;
}

//This function finds the SkeletalAnimation with the name we pass in, we called it
//right after calling our recursiveNodeProcess() function, but this function
//will only really come into play during the next tutorial, where we cover
//the actual SkeletalAnimation portion of skeletal SkeletalAnimation.
aiNodeAnim* FindAiNodeAnim(std::string name)
{
    for(size_t i = 0; i < ai_nodes_anim.size(); i++)
    {
        if(ai_nodes_anim.at(i)->mNodeName.data == name)
        {
            return ai_nodes_anim.at(i);
        }
    }
    return nullptr;
}