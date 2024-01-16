#include "World.h"
#include "Camera.h"
#include "../Shaders/ShaderManager.h"
#include "../Particle/ParticleSystem.h"
#include "Codecs/XMLReader.h"
#include "Graphics/ShaderProgram.h"
#include <System/MemoryManagement.h>
#include <Desktop/Window.h>
#include "../2D/World2D.h"
#include "../3D/World3D.h"


World::World(const Type& type)
{
    iType = type;
    pPhysics = new Physics();
}

World::~World()
{
    Gum::_delete(pPhysics);
    for(size_t i = 0; i < vCamera.size(); i++)
        Gum::_delete(vCamera[i]);
}

void World::update()
{
    pPhysics->update();
    for (size_t i = 0; i < vUpdateables.size(); i++) { vUpdateables[i](); }
}

void World::renderRenderable()
{
    for (size_t i = 0; i < vRenderables.size(); i++) 
    {
        vRenderables[i]->prerender();
        vRenderables[i]->render();
    }
}

void World::renderRenderableIDs()
{
    for (size_t i = 0; i < vRenderables.size(); i++) 
        vRenderables[i]->renderID();
}

void World::renderParticles(ShaderProgram* shader)
{
	for (size_t i = 0; i < vParticles.size(); i++)
	{
		vParticles[i]->update();
		vParticles[i]->render();
	}
}

void World::renderBillboards(ShaderProgram* shader)
{
	for (size_t i = 0; i < vBillboards.size(); i++) 
        vBillboards[i]->render(shader);
}

void World::updateProjection()
{
    for(Renderable* renderable : vRenderables)
        renderable->onProjectionUpdate();
}


World* World::readFromFile(const Gum::Filesystem::File& file)
{    
    XMLReader reader(file);
    XMLNode* rootNode = reader.getRootNode();
    if(rootNode->getAttribute("type") == "2D")
        return World2D::readFromFile(reader);
    
    return World3D::readFromFile(reader);
}


void World::addUpdateable(std::function<void()> updateable) { this->vUpdateables.push_back(updateable); }
void World::addRenderable(Renderable* renderable)           { this->vRenderables.push_back(renderable); }
void World::addBillboard(Billboard* billboard)              { this->vBillboards.push_back(billboard); }
void World::addParticles(ParticleSystem* particles)         { this->vParticles.push_back(particles); }

//
// Setter
//
void World::setName(std::string name)                       { sName = name; }

//
// Getter
//
Camera* World::getCamera(const unsigned int& index)         { return this->vCamera[index]; }
Physics* World::getPhysics()                                { return this->pPhysics; }
World::Type World::getType() const                          { return this->iType; }
std::string World::getName()                                { return this->sName; }