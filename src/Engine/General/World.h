#pragma once
#include "../Object/Object.h"
#include "../Particle/Billboard.h"
#include "../Particle/ParticleSystem.h"
#include "../Managers/LightManager.h"
#include "../Managers/ObjectManager.h"

#include "Camera.h"

#include <functional>

class World
{
private:
	std::vector<std::function<void()>> vUpdateables;
	std::vector<std::function<void()>> vRenderables;
    std::vector<Object*> vObjects;
    std::vector<ParticleSystem*> vParticles;
    std::vector<Billboard*> vBillboards;
    std::vector<Camera*> vCamera;
	ObjectManager* pObjectManager;
	LightManager* pLightManager;

    ShaderProgram* pParticleShader;
    ShaderProgram* pBillboardShader;

public:
    World(const ivec2& resolution, Gum::Window* context);
    ~World();

    void update();
    void renderRenderable();
    void renderSky();
    void renderParticles();
    void renderBillboards();

	void addUpdateable(std::function<void()> updateable);
	void addRenderable(std::function<void()> renderable);
    void addBillboard(Billboard* billboard);

    LightManager* getLightManager();
    ObjectManager* getObjectManager();
    Camera* getCamera(const unsigned int& index = 0);
};