#include "Gizmo.h"
#include "../3D/Camera3D.h"
#include "GizmoShader.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/Variables.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/VertexBufferObject.h"
#include "Primitives/Transformable.h"
#include "Renderer.h"
#include "System/Output.h"
#include <cfloat>

static vec2 worldToPos(const vec4& worldPos, mat4& mat, vec2 position = vec2(0,0), vec2 size = Framebuffer::CurrentlyBoundFramebuffer->getSize())
{
    vec4 trans;
    trans = mat * worldPos;
    trans *= 0.5f / trans.w;
    trans += vec4(0.5f, 0.5f, 0.0f, 0.0f);
    trans.y = 1.f - trans.y;
    trans.x *= size.x;
    trans.y *= size.y;
    trans.x += position.x;
    trans.y += position.y;
    return trans;
}


float mGizmoSizeClipSpace = 0.1f;
static const int halfCircleSegmentCount = 64;
const float screenRotateSize = 0.06f;
const float rotationDisplayFactor = 1.2f;
float mRadiusSquareCenter = 0.0f;


vec4 TransformPoint(const vec4 in, const mat4& matrix)
{
    vec4 out;

    out.x = in.x * matrix[0][0] + in.y * matrix[1][0] + in.z * matrix[2][0] + matrix[3][0];
    out.y = in.x * matrix[0][1] + in.y * matrix[1][1] + in.z * matrix[2][1] + matrix[3][1];
    out.z = in.x * matrix[0][2] + in.y * matrix[1][2] + in.z * matrix[2][2] + matrix[3][2];
    out.w = in.x * matrix[0][3] + in.y * matrix[1][3] + in.z * matrix[2][3] + matrix[3][3];

    return out;
}

static float GetSegmentLengthClipSpace(const vec4& start, const vec4& end, mat4 mvp)
{
    vec4 startOfSegment = start;
    startOfSegment = TransformPoint(startOfSegment, mvp);
    if (fabsf(startOfSegment.w) > FLT_EPSILON) // check for axis aligned with camera direction
    {
        startOfSegment *= 1.f / startOfSegment.w;
    }

    vec4 endOfSegment = end;
    endOfSegment = TransformPoint(endOfSegment, mvp);
    if (fabsf(endOfSegment.w) > FLT_EPSILON) // check for axis aligned with camera direction
    {
        endOfSegment *= 1.f / endOfSegment.w;
    }

    vec4 clipSpaceAxis = endOfSegment - startOfSegment;
    if (Framebuffer::CurrentlyBoundFramebuffer->getAspectRatioWidthToHeight() < 1.0)
        clipSpaceAxis.x *= Framebuffer::CurrentlyBoundFramebuffer->getAspectRatioWidthToHeight();
    else
        clipSpaceAxis.y /= Framebuffer::CurrentlyBoundFramebuffer->getAspectRatioWidthToHeight();
    float segmentLengthInClipSpace = sqrtf(clipSpaceAxis.x * clipSpaceAxis.x + clipSpaceAxis.y * clipSpaceAxis.y);
    return segmentLengthInClipSpace;
}

//#error
void Gizmo::drawTranslationGizmo(int type)
{
    (void)type; //TODO
    vec3 cameraToModelNormalized;

    mat4 viewInverse = mat4::inverse(Camera::getActiveCamera()->getViewMatrix());
    if(Camera::getActiveCamera()->getType() == Camera::CAMERA3D 
    && ((Camera3D*)Camera::getActiveCamera())->getProjectionMode() == Camera3D::PERSPECTIVE)
    {
        cameraToModelNormalized = vec3::normalize(Transformable<float, 3U>::getPosition() - Camera::getActiveCamera()->getPosition());
    }
    else
    {
        cameraToModelNormalized = vec3(-viewInverse[0][2], -viewInverse[1][2], -viewInverse[1][2]);
    }

    cameraToModelNormalized = mat4::inverse(Transformable<float, 3U>::mTransformation) * vec4(cameraToModelNormalized, 1);

    mRadiusSquareCenter = screenRotateSize * Framebuffer::CurrentlyBoundFramebuffer->getSize().y;

    mat4 mModelInverse = mat4::inverse(mTransformation);
    mat4 mViewProjection = Camera::getActiveCamera()->getViewMatrix() * Camera::getActiveCamera()->getProjectionMatrix();
    mat4 mvp = mTransformation * mViewProjection;

    vec4 rightViewInverse = Camera::getActiveCamera()->getStrafeDirection();
    rightViewInverse = mModelInverse * rightViewInverse;
    float rightLength = GetSegmentLengthClipSpace(vec4(0.f, 0.f, 0, 0), rightViewInverse, mvp);
    float mScreenFactor = mGizmoSizeClipSpace / rightLength;
    Gum::Output::print("Screen Factor: " + _str(mScreenFactor) + " rightlen: " + _str(rightLength));
    

    bool hasRSC = false;
    for (unsigned int axis = 0; axis < 3; axis++)
    {
        /*if(!Intersects(op, static_cast<OPERATION>(ROTATE_Z >> axis)))
        {
            continue;
        }*/
        const bool usingAxis = false;//(gContext.mbUsing && type == MT_ROTATE_Z - axis);
        const int circleMul = (hasRSC && !usingAxis ) ? 1 : 2;

        //vec2 circlePos = (vec2)alloca(sizeof(ImVec2) * (circleMul * halfCircleSegmentCount + 1));

        float angleStart = atan2f(cameraToModelNormalized.at((4 - axis) % 3), cameraToModelNormalized.at((3 - axis) % 3)) + (float)GUM_PI * 0.5f;

        vec2 firstpos;
        vec2 oldpos = vec2(0,0);
        for (int i = 0; i < circleMul * halfCircleSegmentCount + 1; i++)
        {
            float ng = angleStart + (float)circleMul * (float)GUM_PI * ((float)i / (float)(circleMul * halfCircleSegmentCount));
            vec4 axisPos = vec4(cosf(ng), sinf(ng), 0.0f, 0.0f);
            vec4 pos = vec4(axisPos.at(axis), axisPos.at((axis + 1) % 3), axisPos.at((axis + 2) % 3), 0.0f) * mScreenFactor * rotationDisplayFactor;
            vec2 circlePos = worldToPos(pos, mvp);
            if(i == 0)
                firstpos = circlePos;
            addLine(circlePos, oldpos);
            oldpos = circlePos;
        }

        /*if (!gContext.mbUsing || usingAxis)
        {
            drawList->AddPolyline(circlePos, circleMul* halfCircleSegmentCount + 1, colors[3 - axis], false, gContext.mStyle.RotationLineThickness);
        }*/

        float radiusAxis = sqrtf((worldToPos(getPosition(), mViewProjection) - firstpos).length());
        if (radiusAxis > mRadiusSquareCenter)
        {
            mRadiusSquareCenter = radiusAxis;
        }
    }
    /*if(hasRSC && (!gContext.mbUsing || type == MT_ROTATE_SCREEN))
    {
        drawList->AddCircle(worldToPos(gContext.mModel.v.position, gContext.mViewProjection), gContext.mRadiusSquareCenter, colors[0], 64, gContext.mStyle.RotationOuterLineThickness);
    }*/

    /*if (gContext.mbUsing && (gContext.mActualID == -1 || gContext.mActualID == gContext.mEditingID) && IsRotateType(type))
    {
        ImVec2 circlePos[halfCircleSegmentCount + 1];

        circlePos[0] = worldToPos(gContext.mModel.v.position, gContext.mViewProjection);
        for (unsigned int i = 1; i < halfCircleSegmentCount + 1; i++)
        {
            matrix_t rotateVectorMatrix;
            rotateVectorMatrix.RotationAxis(gContext.mTranslationPlan, ng);
            vec_t pos;
            pos.TransformPoint(gContext.mRotationVectorSource, rotateVectorMatrix);
            pos *= gContext.mScreenFactor * rotationDisplayFactor;
            circlePos[i] = worldToPos(pos + gContext.mModel.v.position, gContext.mViewProjection);
        }
        drawList->AddConvexPolyFilled(circlePos, halfCircleSegmentCount + 1, GetColorU32(ROTATION_USING_FILL));
        drawList->AddPolyline(circlePos, halfCircleSegmentCount + 1, GetColorU32(ROTATION_USING_BORDER), true, gContext.mStyle.RotationLineThickness);

        ImVec2 destinationPosOnScreen = circlePos[1];
        char tmps[512];
        ImFormatString(tmps, sizeof(tmps), rotationInfoMask[type - MT_ROTATE_X], (gContext.mRotationAngle / (float)GUM_PI) * 180.f, gContext.mRotationAngle);
        drawList->AddText(ImVec2(destinationPosOnScreen.x + 15, destinationPosOnScreen.y + 15), GetColorU32(TEXT_SHADOW), tmps);
        drawList->AddText(ImVec2(destinationPosOnScreen.x + 14, destinationPosOnScreen.y + 14), GetColorU32(TEXT), tmps);
    }*/
}

Gizmo::Gizmo()
{
  pShader = ShaderProgram::requestShaderProgram("GizmoShader", true);
  pShader->addShader(new Shader(GizmoVertexShader, Shader::TYPES::VERTEX_SHADER));
  pShader->addShader(new Shader(GizmoFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pShader->build({ { "vertices", 0 }, { "colors", 1 } });

  pLineVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::LINES);

  pLineVBO = new VertexBufferObject<vec2>();
  pLineVBO->setData(vLineVBOData, Gum::Graphics::DataState::DYNAMIC);
  pLineVAO->addAttribute(pLineVBO, 0, 2, Gum::Graphics::Datatypes::FLOAT, 0, 0);

  drawTranslationGizmo(0);
  
  pLineVBO->setData(vLineVBOData, Gum::Graphics::DataState::DYNAMIC);
  pLineVAO->setVertexCount((unsigned int)vLineVBOData.size());
}

Gizmo::~Gizmo()
{

}

void Gizmo::addCircle(vec2 at, float radius)
{
    float angle = 360.0f;
    float resolution = 300.0f;
    float stepsize = angle / resolution;
    for(float i = 0; i < angle; i += stepsize)
    {
        vLineVBOData.push_back(at + vec2(cos(Gum::Maths::toRadians(i)) * radius, sin(Gum::Maths::toRadians(i)) * radius));
        vLineVBOData.push_back(at + vec2(cos(Gum::Maths::toRadians(i+stepsize)) * radius, sin(Gum::Maths::toRadians(i+stepsize)) * radius));
    }
}

void Gizmo::addLine(vec2 from, vec2 to)
{
    vLineVBOData.push_back(from);
    vLineVBOData.push_back(to);

    //std::cout << "Adding line from " + from.toString() + " to " + to.toString() << std::endl;
}


void Gizmo::onProjectionUpdate()
{
    /*ShaderProgram* oldShader = ShaderProgram::getCurrentlyBoundShader();
    pShader->use();
    pShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());

    if(oldShader != nullptr)
        oldShader->use();*/
}


void Gizmo::render()
{
    pShader->use();
    pLineVAO->bind();
    pLineVAO->render(1);
    pLineVAO->unbind();
    pShader->unuse();
}

void Gizmo::renderID()
{

}