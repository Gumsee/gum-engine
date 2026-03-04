#pragma once
#include <functional>

#include "../Particle/Billboard.h"
#include "../Physics/Physics.h"
#include <Graphics/Renderable.h>

class Camera;
class ParticleSystem;

class World
{
public: 
    enum Type
    {
        WORLD2D,
        WORLD3D,
    };

private:
    Type iType;
    std::string sName;

protected:
	std::vector<std::function<void()>> vUpdatables;
	std::vector<Renderable*> vRenderables;
    std::vector<ParticleSystem*> vParticles;
    std::vector<Billboard*> vBillboards;
    std::vector<Camera*> vCamera;
    Physics* pPhysics;

public:
    World(const Type& type);
    virtual ~World();

    virtual void update();
    virtual void renderSky() {};
    void renderRenderable();
    void renderRenderableIDs();
    void renderParticles(ShaderProgram* shader);
    void renderBillboards(ShaderProgram* shader);
    void updateProjection();

	void addUpdatable(std::function<void()> updatable);
	void addRenderable(Renderable* renderable);
    void addBillboard(Billboard* billboard);
    void addParticles(ParticleSystem* particles);

    virtual void saveToFile(const Gum::Filesystem::File& file) = 0;
    static World* readFromFile(const Gum::Filesystem::File& file);

    //Setter
    void setName(std::string name);

    //Getter
    Camera* getCamera(const unsigned int& index = 0);
    Physics* getPhysics();
    Type getType() const;
    std::string getName();
};