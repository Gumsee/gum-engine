#include "gum-engine.h"
#include "Engine/PostProcessing/PostProcessing.h"
#include "Engine/Particle/ShaderInitializer.h"
#include "Engine/Material/MaterialManager.h"
#include "Engine/Shaders/ShaderManager.h"
#include "Engine/Lightning/LightManager.h"
#include "Engine/Texture/TextureManager.h"
#include "Engine/Shaders/SimpleShader.h"
#include "Engine/Shaders/ThicklinesShader.h"

namespace Gum { 
namespace Engine
{
	void init()
    {
        Gum::MaterialManager::init();
        Gum::PostProcessing::initShaders();
        Gum::Particles::initShaders();
        initSimpleShader();
        initThicklinesShader();
    }

	void cleanup()
    {
        Gum::ShaderManager::cleanup();
        Gum::MaterialManager::cleanup();
        Gum::TextureManager::cleanup();
        LightManager::cleanup();
    }
}};