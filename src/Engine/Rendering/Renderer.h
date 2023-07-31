#pragma once
#include <chrono>

#include <OpenGL/Framebuffer.h>
#include <GUI/Primitives/Box.h>

#include "../PostProcessing/Effects/PostProcessingEffect.h"
#include "../PostProcessing/ImageCorrection/HighDynamicRange.h"

#include "IDRenderer.h"
#include "OcclusionMask.h"

class Renderer
{
private:
    inline static Renderer* pActiveRenderer = nullptr;

protected:
	Framebuffer* pFramebuffer;
    IDRenderer* pIDRenderer;
    Box* pRenderCanvas;
    float fExposure;

	OcclusionMask* pOcclusionMask;
    HighDynamicRange* pHighDynamicRange;
    std::vector<PostProcessingEffect*> vPostProcessingEffects;


	long long microseconds;
	std::chrono::high_resolution_clock::time_point start;

    virtual void renderInternal() {};
    virtual void renderIDsInternal() {};
    virtual void updateFramebufferSize();

public:
    Renderer(Box* canvas);
    ~Renderer();


    void render();
    virtual void update() {};
    void renderIDs();
    void makeActive();

    void addPostProcessingEffect(PostProcessingEffect* effect);

    //Setter
    void setExposure(const float& exposure);
    void setResolution(const ivec2& resolution);
    void setRenderCanvas(Box* canvas);

    //Getter
	long long getExecutionTime() const;
    Box* getRenderCanvas();
    float getExposure() const;
    Framebuffer* getFramebuffer();
    IDRenderer* getIDRenderer();
    static Renderer* getActiveRenderer();
};