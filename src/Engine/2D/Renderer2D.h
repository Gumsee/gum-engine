// #pragma once
// #include <chrono>

// #include <OpenGL/Framebuffer.h>
// #include <GUI/Primitives/Box.h>

// #include "../Lightning/G_Buffer.h"

// #include "../PostProcessing/SSAO.h"
// #include "../PostProcessing/Effects/PostProcessingEffect.h"
// #include "../PostProcessing/ImageCorrection/HighDynamicRange.h"
// #include "../Texture/EnvironmentMap.h"

// #include "Desktop/Window.h"
// #include "IDRenderer.h"
// #include "OcclusionMask.h"
// #include "Grid.h"

// class World2D;
// class ShadowMapping;
// class Lightning;

// class Renderer3D
// {
// private:
// 	Framebuffer* pFramebuffer;
//     HighDynamicRange* pHighDynamicRange;
//     IDRenderer* pIDRenderer;
//     float fExposure;
//     Box* pRenderCanvas;

//     std::vector<PostProcessingEffect*> vPostProcessingEffects;

//     World2D* pWorld;
//     #ifdef DEBUG
//         Grid* pGrid;
//     #endif

// 	//Occlusion Culling
// 	OcclusionMask *pOcclusionMask;
// 	EnvironmentMap *pEnvironmentMap;
// 	ShadowMapping *pShadowMaps;
// 	SSAO *pSSAO;
// 	Lightning *pLightning;
// 	G_Buffer *pGBuffer;

// 	long long microseconds;
// 	std::chrono::high_resolution_clock::time_point start;

// public:
//     Renderer3D(Box* canvas, World2D* world);
//     ~Renderer3D();

//     inline static Renderer3D* ActiveRenderer = nullptr;

//     void render();
//     void renderIDs();
//     void update();

//     void addPostProcessingEffect(PostProcessingEffect* effect);

//     void updateFramebufferSize();

//     //Setter
//     void setExposure(const float& exposure);
//     void setResolution(const ivec2& resolution);
//     void setRenderCanvas(Box* canvas);
//     void setWorld(World2D* world);

//     //Getter
// 	long long getExecutionTime() const;
//     SSAO* getSSAO();
//     G_Buffer* getGBuffer();
//     EnvironmentMap* getEnvironmentMap();
//     ShadowMapping* getShadowMapping();
//     Box* getRenderCanvas();
//     float getExposure() const;
//     World2D* getWorld();
//     Framebuffer* getFramebuffer();
//     IDRenderer* getIDRenderer();
// };