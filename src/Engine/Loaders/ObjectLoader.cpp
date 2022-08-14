#include "ObjectLoader.h"
#include <Essentials/Tools.h>

void ObjectLoader::open(const std::string& fileName)
{
    Assimp::Importer importer;
    pScene = importer.ReadFile(fileName, aiProcess_RemoveRedundantMaterials | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices);
    // check for errors
    if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) // if is Not Zero
    {
        Gum::Output::fatal("ObjectLoader: " + fileName + " ERROR::ASSIMP:: " + importer.GetErrorString());
        return;
    }
    processAnimNode();
    processNode(pScene->mRootNode);

    rootBone = new Bone(vMeshes[0], 0, "root", mat4(), mat4());
    recursiveBuildSkeleton(pScene->mRootNode, rootBone, 0);
    globalInverseTransform = mat4::inverse(Tools::aiMatrix4x4ToMat4(pScene->mRootNode->mTransformation));
    
    Gum::Output::info("Loaded Object: " + fileName + ", numMeshes: " + std::to_string(vMeshes.size()) + ", Children: " + std::to_string(pScene->mRootNode->mNumChildren));
}

void ObjectLoader::open(const aiScene *scene)
{
    this->pScene = scene;
    processAnimNode();
    processNode(pScene->mRootNode);

    rootBone = new Bone(vMeshes[0], 0, "root", mat4(), mat4());
    recursiveBuildSkeleton(pScene->mRootNode, rootBone, 0);
    globalInverseTransform = mat4::inverse(Tools::aiMatrix4x4ToMat4(pScene->mRootNode->mTransformation));
}

void ObjectLoader::recursiveBuildSkeleton(aiNode* node, Bone *currentBone, int layer)
{
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        std::string ChildBoneName = node->mChildren[i]->mName.data;
        Bone *childBone = FindBone(ChildBoneName);
        if(childBone != nullptr)
        {
            childBone->setParent(currentBone);
            currentBone->addChild(childBone);
            recursiveBuildSkeleton(node->mChildren[i], childBone, layer + 1);
        }
        else 
        {
            recursiveBuildSkeleton(node->mChildren[i], currentBone, layer + 1);
        }
    }
}

//We only get data from the first mSkeletalAnimation because 
//Assimp crushes all of the SkeletalAnimation data into one
//large sequence of data known as mSkeletalAnimation.
//Assimp does not support multiple mSkeletalAnimations, surprisingly.
void ObjectLoader::processAnimNode()
{
    if(pScene->mNumAnimations != 0)
        for(unsigned int i = 0; i < pScene->mAnimations[0]->mNumChannels; i++)
            ai_nodes_anim.push_back(pScene->mAnimations[0]->mChannels[i]);
}

void ObjectLoader::processNode(aiNode *node)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];
        Mesh *retMesh = processMesh(mesh);

        retMesh->offsetMatrix = Tools::aiMatrix4x4ToMat4(pScene->mRootNode->mTransformation) * Tools::aiMatrix4x4ToMat4(node->mTransformation);
        
    }
    
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i]);
    }
}

void ObjectLoader::processBones(Mesh* mesh, aiMesh *pAiMesh)
{
    for (uint i = 0 ; i < pAiMesh->mNumBones ; i++) 
    {
        std::string BoneName = pAiMesh->mBones[i]->mName.data;
        int BoneIndex = 0;


        if (FindBone(BoneName) == nullptr) 
        {
            BoneIndex = i;
        }
        else 
        {
            BoneIndex = FindBone(BoneName)->getID();
        }

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

Mesh* ObjectLoader::processMesh(aiMesh *mesh)
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

    processBones(pMesh, mesh);
    
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
Bone* ObjectLoader::FindBone(std::string name)
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
aiNodeAnim* ObjectLoader::FindAiNodeAnim(std::string name)
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

const aiScene* ObjectLoader::getScene()
{
    return pScene;
}