#pragma once
#include <chrono>

#include <OpenGL/Framebuffer.h>
#include <GUI/Primitives/Box.h>

#include "../Lightning/G_Buffer.h"

#include "../PostProcessing/SSAO.h"
#include "../PostProcessing/Effects/PostProcessingEffect.h"
#include "../PostProcessing/ImageCorrection/HighDynamicRange.h"
#include "../Texture/EnvironmentMap.h"

#include "Essentials/Window.h"
#include "OcclusionMask.h"
#include "Grid.h"

class World;
class ShadowMapping;
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
    Gum::Window* pContextWindow;
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
    Renderer3D(Box* canvas, Gum::Window* context);
    ~Renderer3D();

    static Renderer3D* ActiveRenderer;

    void render();
    void update();

    void addPostProcessingEffect(PostProcessingEffect* effect);

    void updateFramebufferSize();

    //Setter
    void setExposure(const float& exposure);
    void setResolution(const ivec2& resolution);
    void setRenderCanvas(Box* canvas);
    void setWorld(World* world);

    //Getter
	long long getExecutionTime() const;
    Gum::Window* getContextWindow();
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