#include "Lightning.h"
#include <random>
#include <chrono>
#include <Essentials/MemoryManagement.h>
#include "LightningShader.h"
#include "../Managers/ShaderManager.h"
#include "../General/Renderer3D.h"
#include "../General/World.h"
#include "ShadowMapping/ShadowMapping.h"

Lightning::Lightning(Box* canvas, Renderer3D *renderer)
{
	this->pShader = nullptr;
    this->pRenderCanvas = canvas;
	this->pRenderer = renderer;
	initShader();
}


Lightning::~Lightning() 
{ 
	pShader->removeShader(0);
	Gum::_delete(pShader);
}


void Lightning::render(ShadowMapping *shadowmap, World* world)
{
	auto start = std::chrono::high_resolution_clock::now();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    pRenderer->getGBuffer()->getPositionMap()->bind(0);
	pRenderer->getGBuffer()->getNormalMap()->bind(1);
	pRenderer->getGBuffer()->getDiffuseMap()->bind(2);
	pRenderer->getSSAO()->getResultTexture()->bind(3);
	pRenderer->getGBuffer()->getObjectDataMap()->bind(4);
	//pRenderer->getEnvironmentMap()->getTexture()->bind(5);
    world->getObjectManager()->getSkybox()->getTexture()->bind(5);
	world->getObjectManager()->getSkybox()->getIrradianceMap()->bind(6);
	world->getObjectManager()->getSkybox()->getPreFilterMap()->bind(7);
	world->getObjectManager()->getSkybox()->getBRDFConvMap()->bind(8);
	shadowmap->getResultTexture(0)->bind(9);

	pShader->use();
	// for (int i = 0; i < lights->getNearestPointLights().size(); i++)
	// {
	// 	pShader->LoadUniform("lightPosition[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getPosition());
	// 	pShader->LoadUniform("lightColor[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getColor());
	// 	pShader->LoadUniform("lightAttenuation[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getAttenuation());
	// }

	pixelSize = vec2(1.0f) / pRenderer->getRenderCanvas()->getSize();

	const float linear = 0.7;
	const float quadratic = 1.8;
	const float constant = 1.0;

	for (unsigned int i = 0; i < world->getLightManager()->getNearestPointLights().size(); i++)
	{
		pShader->LoadUniform("lights[" + std::to_string(i) + "].Position", world->getLightManager()->getNearestPointLights()[i]->getPosition());
		pShader->LoadUniform("lights[" + std::to_string(i) + "].Color", world->getLightManager()->getNearestPointLights()[i]->getColor());
		pShader->LoadUniform("lights[" + std::to_string(i) + "].Linear", linear);
		pShader->LoadUniform("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		
		vec3 color = world->getLightManager()->getNearestPointLights()[i]->getColor();
		const float maxBrightness = std::max(std::max(color.x, color.y), color.z);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

		pShader->LoadUniform("lights[" + std::to_string(i) + "].Radius", radius);
	}
	pShader->LoadUniform("numLights", (int)world->getLightManager()->getNearestPointLights().size());

	pShader->LoadUniform("SunColor", world->getLightManager()->getSun()->getColor());
	pShader->LoadUniform("SunDirection", world->getLightManager()->getSun()->getDirection());
	pShader->LoadUniform("viewmat", Camera::ActiveCamera->getViewMatrix());
	pShader->LoadUniform("viewPos", Camera::ActiveCamera->getPosition());
	pShader->LoadUniform("viewDir", Camera::ActiveCamera->getViewDirection());
	pShader->LoadUniform("ToShadowMap", *shadowmap->getMatrix());
	pShader->LoadUniform("ShadowMapSize", *shadowmap->getShadowMapSize());
	pShader->LoadUniform("pixelSize", pixelSize);
	pRenderCanvas->renderCustom();

	pShader->unuse();

    for(int i = 0; i < 9; i++)
    {
        //glActiveTexture(GL_TEXTURE0 + i);
        //glBindTexture(GL_TEXTURE_2D, 0);
    }
    
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

void Lightning::cleanup()
{
	
}


//
// Getter
//
long long Lightning::getExecutionTime()  { return microseconds; }

void Lightning::initShader()
{
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram();
        pShader->addShader(Gum::ShaderManager::getShader("PostProcessingShaderVert"));
        pShader->addShader(new Shader(LightningFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("LightningShader");

        //Textures
        pShader->addTexture("gPosition", 0);
        pShader->addTexture("gNormal", 1);
        pShader->addTexture("gAlbedo", 2);
        pShader->addTexture("ssao", 3);
        pShader->addTexture("gObjectData", 4);
        pShader->addTexture("ReflectionMap", 5);
        pShader->addTexture("IrradianceMap", 6);
        pShader->addTexture("prefilterMap", 7);
        pShader->addTexture("brdfLUT", 8);
        pShader->addTexture("ShadowMap", 9);

        pShader->addUniform("viewmat");
        pShader->addUniform("viewPos");
        pShader->addUniform("viewDir");
        pShader->addUniform("ToShadowMap");
        pShader->addUniform("ShadowMapSize");
        pShader->addUniform("numLights");
        pShader->addUniform("pixelSize");

        for (int i = 0; i < Settings::getSetting(Settings::Names::NUM_ACTIVE_LIGHTS); i++)
        {
            pShader->addUniform("lights[" + std::to_string(i) + "].Position");
            pShader->addUniform("lights[" + std::to_string(i) + "].Color");
            pShader->addUniform("lights[" + std::to_string(i) + "].Radius");
            pShader->addUniform("lights[" + std::to_string(i) + "].Quadratic");		
            pShader->addUniform("lights[" + std::to_string(i) + "].Linear");		
        }
        pShader->addUniform("SunColor");
        pShader->addUniform("SunDirection");
    }
}