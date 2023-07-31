#pragma once
#include <functional>

#include "../Particle/Billboard.h"
#include "../Physics/Physics.h"
#include <OpenGL/Renderable.h>

class Camera;
class ParticleSystem;

class World
{
protected:
	//std::vector<std::function<void()>> vUpdateables;
	std::vector<Renderable*> vRenderables;
    std::vector<ParticleSystem*> vParticles;
    std::vector<Billboard*> vBillboards;
    std::vector<Camera*> vCamera;
    Physics* pPhysics;

public:
    World();
    ~World();

    virtual void update();
    virtual void renderSky() {};
    void renderRenderable();
    void renderRenderableIDs();
    void renderParticles(ShaderProgram* shader);
    void renderBillboards(ShaderProgram* shader);
    void updateProjection();

	//void addUpdateable(std::function<void()> updateable);
	void addRenderable(Renderable* renderable);
    void addBillboard(Billboard* billboard);
    void addParticles(ParticleSystem* particles);

    //Getter
    Camera* getCamera(const unsigned int& index = 0);
    Physics* getPhysics();
};