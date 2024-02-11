#pragma once
#include "../Lightning/G_Buffer.h"
#include "../PostProcessing/SSAO.h"
#include "../Texture/EnvironmentMap.h"

#include "../Rendering/Renderer.h"
#include "Grid.h"

class World3D;
class ShadowMapping;
class Lightning;

class Renderer3D : public Renderer
{
private:
    World3D* pWorld;
   // #ifdef DEBUG
        Grid* pGrid;
    //#endif

	//Occlusion Culling
	EnvironmentMap *pEnvironmentMap;
	ShadowMapping *pShadowMaps;
	SSAO *pSSAO;
	Lightning *pLightning;
	G_Buffer *pGBuffer;
    
    ShaderProgram* pParticleShader;
    ShaderProgram* pBillboardShader;


    void renderInternal()  override;
    void renderIDsInternal()  override;

public:
    Renderer3D(Box* canvas);
    ~Renderer3D();

    void update() override;
    void updateFramebufferSize() override;

    //Setter
    void setWorld(World3D* world);

    //Getter
    SSAO* getSSAO();
    G_Buffer* getGBuffer();
    EnvironmentMap* getEnvironmentMap();
    ShadowMapping* getShadowMapping();
    World3D* getWorld();
};