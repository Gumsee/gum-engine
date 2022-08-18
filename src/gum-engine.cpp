#include "gum-engine.h"
#include "Engine/PostProcessing/PostProcessing.h"
#include "Engine/Particle/ShaderInitializer.h"
#include "Engine/Managers/MaterialManager.h"
#include "Engine/Managers/ShaderManager.h"
#include "Engine/Managers/LightManager.h"
#include "Engine/Managers/TextureManager.h"

namespace Gum { 
namespace Engine
{
	void init()
    {
        Gum::MaterialManager::init();
        Gum::PostProcessing::initShaders();
        Gum::Particles::initShaders();
    }

	void cleanup()
    {
        Gum::ShaderManager::cleanup();
        Gum::MaterialManager::cleanup();
        Gum::TextureManager::cleanup();
        LightManager::cleanup();
    }
}};