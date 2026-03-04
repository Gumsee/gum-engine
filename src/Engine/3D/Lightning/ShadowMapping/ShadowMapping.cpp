#include "ShadowMapping.h"
#include "Essentials/Settings.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/TextureDepth3D.h"
#include "ShadowMappingShader.h"
#include "../../Camera3D.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <Graphics/WrapperFunctions.h>


ShadowMapping::ShadowMapping()
{
    initShader();

    vShadowCascadeLevels = { 
        Settings::getSetting(Settings::RENDERDISTANCE) / 250.0f, 
        Settings::getSetting(Settings::RENDERDISTANCE) / 25.0f, 
        Settings::getSetting(Settings::RENDERDISTANCE) / 10.0f, 
        Settings::getSetting(Settings::RENDERDISTANCE) / 2.0f
    };
    vLightMatrices.resize(vShadowCascadeLevels.size() + 1);

    pFramebuffer = new Framebuffer(ivec2(Settings::getSetting(Settings::SHADOW_SIZE)));
    TextureDepth3D* depthtex = (TextureDepth3D*)pFramebuffer->addDepthTextureArrayAttachment(vShadowCascadeLevels.size()+1);
    depthtex->setBordercolor(rgba(255,255,255,255));
}


ShadowMapping::~ShadowMapping() 
{
	Gum::_delete(pFramebuffer);
}


void ShadowMapping::render(const vec3& lightdir, const std::function<void()>& renderfunc)
{
    pFramebuffer->bind();
    pFramebuffer->clear(Framebuffer::ClearFlags::DEPTH);
    Gum::Graphics::cullBackside(false);
    pShader->use();
    getLightSpaceMatrices(lightdir);
    pShader->loadUniform("lightSpaceMatrices", vLightMatrices);

    renderfunc();
    pShader->unuse();
    Gum::Graphics::cullBackside(true);
    pFramebuffer->unbind();
}

std::vector<vec4> ShadowMapping::getFrustumCornersWorldSpace(mat4& proj, const mat4& view)
{
    mat4 inv = mat4::inverse(proj * view);
    
    std::vector<vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const vec4 pt = inv * vec4(
                    2.0f * x - 1.0f,
                    2.0f * y - 1.0f,
                    2.0f * z - 1.0f,
                    1.0f
                );
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }
    
    return frustumCorners;
}

mat4 ShadowMapping::getLightSpaceMatrix(const vec3& lightdir, const float nearPlane, const float farPlane)
{
    mat4 proj = Gum::Maths::perspective(Camera::getActiveCamera()->getFOV() * 1.0f, Framebuffer::WindowFramebuffer->getAspectRatioWidthToHeight(), nearPlane, farPlane);
    std::vector<vec4> corners = getFrustumCornersWorldSpace(proj, Camera::getActiveCamera()->getViewMatrix());

    vec3 center = vec3(0, 0, 0);
    for (const vec4& v : corners)
        center += vec3(v);
    center /= (float)corners.size();

    mat4 lightView = Gum::Maths::view(center - lightdir, center, Camera::getActiveCamera()->getUpDirection());

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const vec4& v : corners)
    {
        const vec4 trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 10.0f; // 10.0f;
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }

    //minX = left
    //maxX = right
    //minY = bottom
    //maxY = top
    //minZ = near
    //maxZ = far

    mat4 lightProjection = Gum::Maths::ortho(maxY, maxX, minY, minY, minZ, maxZ);
    return lightProjection * lightView;
}

void ShadowMapping::getLightSpaceMatrices(const vec3& lightdir)
{
    for (size_t i = 0; i < vShadowCascadeLevels.size() + 1; ++i)
    {
        if (i == 0)
        {
            vLightMatrices[i] = getLightSpaceMatrix(lightdir, Camera3D::NEAR_PLANE, vShadowCascadeLevels[i]);
        }
        else if (i < vShadowCascadeLevels.size())
        {
            vLightMatrices[i] = getLightSpaceMatrix(lightdir, vShadowCascadeLevels[i - 1], vShadowCascadeLevels[i]);
        }
        else
        {
            vLightMatrices[i] = getLightSpaceMatrix(lightdir, vShadowCascadeLevels[i - 1], Settings::getSetting(Settings::RENDERDISTANCE));
        }
    }
}

//
// Getter
//
ShaderProgram* ShadowMapping::getShader() 	          { return pShader; }
std::vector<mat4>& ShadowMapping::getMatrices()       { return vLightMatrices; }
Texture* ShadowMapping::getResultTexture()            { return pFramebuffer->getDepthTextureAttachment(); }
std::vector<float>& ShadowMapping::getCascadeLevels() { return vShadowCascadeLevels; }


void ShadowMapping::initShader()
{
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram("ShadowMapShader", true);
        pShader->addShader(new Shader(ShadowMappingVertexShader, Shader::TYPES::VERTEX_SHADER));
        pShader->addShader(new Shader(ShadowMappingGeometryShader, Shader::TYPES::GEOMETRY_SHADER));
        pShader->addShader(new Shader(ShadowMappingFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build();
    }
}