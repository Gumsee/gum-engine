#include "Bone.h"
#include <gum-maths.h>
#include <System/Output.h>
#include <Essentials/Tools.h>


Bone::Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, mat4 in_o_mat, mat4 transform)
{
    iID = in_id;
    sName = in_name;
    m4OffsetMatrix = in_o_mat;

    mesh = in_mesh;

    TranformationMatrix = transform;

    pParentBone = nullptr;
}


//All of these Find functions return an integer because they aren't returning
//keyframes themselves, they are returning indices to the current keyframe
//within our stored array of keyframes, animNode.
unsigned int Bone::FindPosition(float time)
{
    for(unsigned int i = 0; i < Times.size() - 1; i++)
        if(time < (float)Times[i + 1])
            return i;
    return 0;
}

unsigned int Bone::FindRotation(float time)
{
    for(unsigned int i = 0 ; i < Times.size() - 1; i++)
        if(time < (float)Times[i + 1])
            return i;
    return 0;
}

vec3 Bone::CalcInterpolatedPosition(float time)
{
    if(Times.size() == 1)
        return PosKeys[0];

    unsigned int PositionIndex = FindPosition(time);
    unsigned int NextPositionIndex = PositionIndex + 1;

    float DeltaTime = Times[NextPositionIndex] - Times[PositionIndex];
    float factor = (time - (float)Times[PositionIndex]) / DeltaTime;

    const vec3 start = PosKeys[PositionIndex];
    const vec3 end = PosKeys[NextPositionIndex];

    return start;//vec3::mix(start, end, factor);
}


quat Bone::CalcInterpolatedRotation(float time)
{
    if(Times.size() == 1)
        return RotKeys[0];

    unsigned int RotationIndex = FindRotation(time);
    unsigned int NextRotationIndex = RotationIndex + 1;

    float DeltaTime = Times[NextRotationIndex] - Times[RotationIndex];
    float factor = (time - (float)Times[RotationIndex]) / DeltaTime;

    const quat start = RotKeys[RotationIndex];
    const quat end = RotKeys[NextRotationIndex];

    return start;//quat::slerp(start, end, factor);
}

//This function will update our node's transformation to match that of
//our keyframes.
void Bone::UpdateKeyframeTransform(float time)
{
    if(Times.size() == 0)
        return;
    TranformationMatrix = 
        Gum::Maths::translateMatrix(CalcInterpolatedPosition(time)) * 
        mat4(Gum::Maths::rotateMatrix(CalcInterpolatedRotation(time))) * 
        Gum::Maths::scaleMatrix(vec3(1.0f));
    
    /*TranformationMatrix = Math::createTransformationMatrix(
        CalcInterpolatedPosition(time), 
        CalcInterpolatedRotation(time), 
        vec3(1.0f)
    );*/
}


void Bone::addChild(Bone* bone)              { this->vChildren.push_back(bone); }

void Bone::setParent(Bone* bone)             { this->pParentBone = bone; }
void Bone::setNodeAnim(aiNodeAnim* nodeanim)
{
    if(nodeanim != nullptr)
    { 
        PosKeys.clear();
        RotKeys.clear();
        Times.clear();
        for(size_t k = 0; k < nodeanim->mNumPositionKeys; k++)
        {
            PosKeys.push_back(Tools::aiVector3ToVec3(nodeanim->mPositionKeys[k].mValue));
            RotKeys.push_back(Tools::aiQuaternionToQuat(nodeanim->mRotationKeys[k].mValue));
            Times.push_back(nodeanim->mPositionKeys[k].mTime);
        }
    }
    else
    {
        Gum::Output::warn("No SkeletalAnimations were found for " + sName);
    }
}

Bone* Bone::getParent()          { return this->pParentBone; }
mat4 Bone::getOffsetMatrix()     { return this->m4OffsetMatrix; }
mat4 Bone::getTransform()        { return this->TranformationMatrix; }
unsigned int Bone::getID()       { return this->iID; }
Bone* Bone::getChild(int index)  { return this->vChildren[index]; }
std::string Bone::getName()      { return this->sName; }
unsigned int Bone::numChildren() { return this->vChildren.size(); }