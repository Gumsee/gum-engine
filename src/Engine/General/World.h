#pragma once
#include <functional>

#include "../Object/Object.h"
#include "../Particle/Billboard.h"
#include "../Managers/LightManager.h"
#include "../Managers/ObjectManager.h"
#include "../Physics/Physics.h"

class Camera;
class ParticleSystem;

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
    Physics* pPhysics;

    ShaderProgram* pParticleShader;
    ShaderProgram* pBillboardShader;

public:
    World(Gum::Window* context);
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
    Physics* getPhysics();
};