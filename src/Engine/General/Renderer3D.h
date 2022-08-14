#pragma once
#include <chrono>

#include <OpenGL/Framebuffer.h>
#include <GUI/Primitives/Box.h>

#include "../PostProcessing/SSAO.h"
#include "../PostProcessing/G_Buffer.h"
#include "../PostProcessing/Lightning.h"
#include "../PostProcessing/OutlineRenderer.h"
#include "../PostProcessing/Effects/PostProcessingEffect.h"
#include "../PostProcessing/ImageCorrection/HighDynamicRange.h"
#include "../PostProcessing/ShadowMapping/ShadowMapping.h"
#include "../Texture/EnvironmentMap.h"

#include "World.h"
#include "OcclusionMask.h"
#include "Grid.h"

class Lightning;

class Renderer3D
{
private:
	Framebuffer* pFramebuffer;
    HighDynamicRange* pHighDynamicRange;
    float fExposure;
    float fAspectRatio;
    Box* pRenderCanvas;

    std::vector<PostProcessingEffect*> vPostProcessingEffects;

    World* pWorld;
    #ifdef DEBUG
        Grid* pGrid;
    #endif

	//Occlusion Culling
	OcclusionMask *pOcclusionMask;
	EnvironmentMap *pEnvironmentMap;
	ShadowMapping *pShadowMaps;
	SSAO *pSSAO;
	Lightning *pLightning;
	G_Buffer *pGBuffer;

	long long microseconds;
	std::chrono::high_resolution_clock::time_point start;

public:
    Renderer3D(Box* canvas);
    ~Renderer3D();

    void render();
    void update();

    void addPostProcessingEffect(PostProcessingEffect* effect);

    //Setter
    void setExposure(const float& exposure);
    void setResolution(const ivec2& resolution);
    void setRenderCanvas(Box* canvas);
    void setWorld(World* world);

    //Getter
	long long getExecutionTime() const;
    SSAO* getSSAO();
    G_Buffer* getGBuffer();
    EnvironmentMap* getEnvironmentMap();
    ShadowMapping* getShadowMapping();
    Box* getRenderCanvas();
    float getExposure() const;
    float getAspectRatio() const;
    World* getWorld();
    Framebuffer* getFramebuffer();
};