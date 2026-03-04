#pragma once
#include <chrono>

#include <Graphics/Framebuffer.h>
#include "Canvas.h"

#include "../PostProcessing/Effects/PostProcessingEffect.h"
#include "../PostProcessing/ImageCorrection/HighDynamicRange.h"

#include "IDRenderer.h"

class Renderer
{
public: 
    enum Type
    {
        RENDERER2D,
        RENDERER3D,
    };

private:
    inline static Renderer* pActiveRenderer = nullptr;
    Type iType;

protected:
	Framebuffer* pFramebuffer;
    IDRenderer* pIDRenderer;
    Canvas* pRenderCanvas;
    float fExposure;

    HighDynamicRange* pHighDynamicRange;
    std::vector<PostProcessingEffect*> vPostProcessingEffects;


	long long microseconds;
	std::chrono::high_resolution_clock::time_point start;

    virtual void renderInternal() {};
    virtual void renderIDsInternal() {};

public:
    Renderer(Canvas* canvas, const Type& type);
    virtual ~Renderer();


    virtual void update() {};
    virtual void updateFramebufferSize();
    void render();
    void renderIDs();
    void makeActive();

    void addPostProcessingEffect(PostProcessingEffect* effect);

    //Setter
    void setExposure(const float& exposure);
    void setRenderCanvas(Canvas* canvas);

    //Getter
	long long getExecutionTime() const;
    Canvas* getRenderCanvas();
    float getExposure() const;
    Type getType() const;
    Framebuffer* getFramebuffer();
    HighDynamicRange* getHighDynamicRange();
    IDRenderer* getIDRenderer();
    static Renderer* getActiveRenderer();
};