#include "gum-engine.h"
#include "Engine/PostProcessing/PostProcessing.h"
#include "Engine/Particle/ShaderInitializer.h"
#include "Engine/Material/MaterialManager.h"
#include "Engine/Lightning/LightManager.h"
#include "Engine/Shaders/SimpleShader.h"
#include "Engine/Shaders/ThicklinesShader.h"
#include <Primitives/Mesh.h>

namespace Gum { 
namespace Engine
{
    bool ENGINE_INITIALIZED = false;
	void init()
    {
        if(ENGINE_INITIALIZED)
            return;
        Gum::MaterialManager::init();
        Gum::PostProcessing::initShaders();
        Gum::Particles::initShaders();
        initSimpleShader();
        initThicklinesShader();

        ENGINE_INITIALIZED = true;
    }

	void cleanup()
    {
        if(!ENGINE_INITIALIZED)
            return;
        Gum::MaterialManager::cleanup();
        Texture::cleanupAllLoadedTextures();
        ShaderProgram::destroyAllShaders();
        LightManager::cleanup();
        Mesh::destroyAllMeshes();

        ENGINE_INITIALIZED = false;
    }
}};