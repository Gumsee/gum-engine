// #pragma once
// #include <functional>

// #include <OpenGL/Object3D.h>
// #include <OpenGL/Sprite2D.h>
// #include "../Particle/Billboard.h"
// #include "../Managers/LightManager.h"
// #include "../Managers/ObjectManager.h"
// #include "../Physics/Physics.h"

// class Camera;
// class ParticleSystem;

// class World2D
// {
// private:
// 	std::vector<std::function<void()>> vUpdateables;
// 	std::vector<Renderable*> vRenderables;
//     std::vector<ParticleSystem*> vParticles;
//     std::vector<Billboard*> vBillboards;
//     std::vector<Sprite2D*> vSprites;
//     std::vector<Camera*> vCamera;
// 	ObjectManager* pObjectManager;
// 	LightManager* pLightManager;
//     Physics* pPhysics;

// public:
//     World2D();
//     ~World2D();

//     void update();
//     void renderRenderable();
//     void renderRenderableIDs();
//     void renderSky();
//     void renderParticles();
//     void renderBillboards();
//     void updateProjection();

// 	void addUpdateable(std::function<void()> updateable);
// 	void addRenderable(Renderable* renderable);
//     void addBillboard(Billboard* billboard);
//     void addParticles(ParticleSystem* particles);

//     LightManager* getLightManager();
//     ObjectManager* getObjectManager();
//     Camera* getCamera(const unsigned int& index = 0);
//     Physics* getPhysics();
// };