#pragma once
#include "Graphics/Renderable.h"
#include "../AnimatedModel.h"

class BoneRenderer : public Renderable
{
private:
    inline static ShaderProgram* pShader = nullptr;
    Object3D* pBoneObject;

    AnimatedModel* pModel;

    void recursiveUpdateBoneMats(Bone *currentBone, mat4 parentTransform);

public:
    BoneRenderer(AnimatedModel* model);
    ~BoneRenderer();

    void render() override;
};