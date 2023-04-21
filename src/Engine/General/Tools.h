#pragma once
#include <Maths/quat.h>
#include <Maths/mat.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>

namespace Tools
{
    inline static vec3 aiVector3ToVec3(const aiVector3D &v)      { return vec3(v.x, v.y, v.z); }
    inline static quat aiQuaternionToQuat(const aiQuaternion &q) { return quat(q.w, q.x, q.y, q.z); }
    inline static mat4 aiMatrix4x4ToMat4(aiMatrix4x4 from)
    {
        mat4 to;
        to[0][0] = (float)from.a1; to[0][1] = (float)from.b1;  to[0][2] = (float)from.c1; to[0][3] = (float)from.d1;
        to[1][0] = (float)from.a2; to[1][1] = (float)from.b2;  to[1][2] = (float)from.c2; to[1][3] = (float)from.d2;
        to[2][0] = (float)from.a3; to[2][1] = (float)from.b3;  to[2][2] = (float)from.c3; to[2][3] = (float)from.d3;
        to[3][0] = (float)from.a4; to[3][1] = (float)from.b4;  to[3][2] = (float)from.c4; to[3][3] = (float)from.d4;

        return to;
    }

    inline static aiMatrix4x4 Mat4ToAiMatrix4(mat4 mat)
    {
        return aiMatrix4x4(mat[0][0],mat[0][1],mat[0][2],mat[0][3],
                        mat[1][0],mat[1][1],mat[1][2],mat[1][3],
                        mat[2][0],mat[2][1],mat[2][2],mat[2][3],
                        mat[3][0],mat[3][1],mat[3][2],mat[3][3]);
    }
};