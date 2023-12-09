#include "Lightning.h"
#include <random>
#include <chrono>
#include <System/MemoryManagement.h>
#include "LightningShader.h"
#include "../../Shaders/ShaderManager.h"
#include "../../PostProcessing/PostProcessing.h"
#include "../Renderer3D.h"
#include "../World3D.h"
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


void Lightning::render(ShadowMapping *shadowmap, World3D* world)
{
	auto start = std::chrono::high_resolution_clock::now();

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	//shadowmap->getResultTexture(0)->bind(9);

	pShader->use();
	// for (int i = 0; i < lights->getNearestPointLights().size(); i++)
	// {
	// 	pShader->loadUniform("lightPosition[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getPosition());
	// 	pShader->loadUniform("lightColor[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getColor());
	// 	pShader->loadUniform("lightAttenuation[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getAttenuation());
	// }

	pixelSize = vec2(1.0f) / pRenderer->getRenderCanvas()->getSize();

	pShader->loadUniform("numLights", (int)world->getLightManager()->numPointLights());

	pShader->loadUniform("SunColor", world->getLightManager()->getSun()->getColor());
	pShader->loadUniform("SunDirection", world->getLightManager()->getSun()->getDirection());
	pShader->loadUniform("viewmat", Camera::getActiveCamera()->getViewMatrix());
	pShader->loadUniform("viewPos", Camera::getActiveCamera()->getPosition());
	pShader->loadUniform("viewDir", Camera::getActiveCamera()->getViewDirection());
	pShader->loadUniform("ToShadowMap", *shadowmap->getMatrix());
	pShader->loadUniform("ShadowMapSize", *shadowmap->getShadowMapSize());
	pShader->loadUniform("pixelSize", pixelSize);
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

//
// Getter
//
long long Lightning::getExecutionTime()  { return microseconds; }

void Lightning::initShader()
{
    if(pShader == nullptr)
    {
        pShader = new ShaderProgram(true);
        pShader->addShader(Gum::ShaderManager::getShader(Gum::PostProcessing::iVertexShaderID));
        pShader->addShader(new Shader(LightningFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
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

        for (int i = 0; i < GUM_MAX_LIGHTS; i++)
        {
            pShader->addUniform("lights[" + std::to_string(i) + "].Position");
            pShader->addUniform("lights[" + std::to_string(i) + "].Color");		
        }
        pShader->addUniform("SunColor");
        pShader->addUniform("SunDirection");
    }
}

void Lightning::loadLight(Light* light, int index)
{
	if(index > GUM_MAX_LIGHTS)
	{
		Gum::Output::error("Maximum number of light exceeded");
		return;
	}

	pShader->use();
	pShader->loadUniform("lights[" + std::to_string(index) + "].Position", light->getPosition());
	pShader->loadUniform("lights[" + std::to_string(index) + "].Color", light->getColor());
	/*
	const float linear = 0.7;
	const float quadratic = 1.8;
	const float constant = 1.0;
	pShader->loadUniform("lights[" + std::to_string(index) + "].Linear", linear);
	pShader->loadUniform("lights[" + std::to_string(index) + "].Quadratic", quadratic);
	
	vec3 color = light->getColor();
	const float maxBrightness = std::max(std::max(color.x, color.y), color.z);
	float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

	pShader->loadUniform("lights[" + std::to_string(index) + "].Radius", radius);*/
	pShader->unuse();
}