#pragma once
#include <map>
#include <string>
#include <Essentials/World.h>

class Examples
{
private:
    World* createAnimatedModelExample();
    World* createBillboardsExample();
    World* createGBufferExample();
    World* createGUIsExample();
    World* createGumMathsExample();
    World* createIrradiancePBRExample();
    World* createMazeLearningExample();
    World* createOceanExample();
    World* createParticlesExample();
    World* createPhysicallyBasedRenderingExample();
    World* createPhysicsExample();
    World* createPostProcessingExample();
    World* createShadowMappingExample();
    World* createSphereGeneratorExample();

    std::map<std::string, World*> mWorlds;

    World* getExampleWorld(std::string name);

public:
    Examples();
    ~Examples();
};