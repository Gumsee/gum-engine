#pragma once
#include "../Lightning/G_Buffer.h"
#include "../Rendering/Renderer.h"

class World2D;

class Renderer2D : public Renderer
{
private:
    World2D* pWorld;
	G_Buffer *pGBuffer;

    ShaderProgram* pParticleShader;
    ShaderProgram* pBillboardShader;
    ShaderProgram* pParallaxSkyShader;

    void renderInternal()  override;
    void renderIDsInternal()  override;
    void initShader();

public:
    Renderer2D(Box* canvas);
    ~Renderer2D();

    void update() override;
    void updateFramebufferSize() override;

    //Setter
    void setWorld(World2D* world);

    //Getter
    G_Buffer* getGBuffer();
    World2D* getWorld();
};