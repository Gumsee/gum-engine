#include "Lightning.h"
#include <random>
#include <chrono>
#include <System/MemoryManagement.h>
#include "Essentials/Settings.h"
#include "LightningShader.h"
#include "../../PostProcessing/PostProcessing.h"
#include "../Renderer3D.h"
#include "../World3D.h"
#include "ShadowMapping/ShadowMapping.h"

Lightning::Lightning(Canvas* canvas)
{
    this->pRenderCanvas = canvas;
	  initShader();
}


Lightning::~Lightning() 
{
}


void Lightning::updateShader(ShadowMapping *shadowmap, World3D* world)
{
	//auto start = std::chrono::high_resolution_clock::now();

	pShader->use();
	// for (int i = 0; i < lights->getNearestPointLights().size(); i++)
	// {
	// 	pShader->loadUniform("lightPosition[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getPosition());
	// 	pShader->loadUniform("lightColor[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getColor());
	// 	pShader->loadUniform("lightAttenuation[" + std::to_string(i) + "]", *lights->getNearestPointLights()[i]->getAttenuation());
	// }

	pixelSize = vec2(1.0f) / pRenderCanvas->getSize();

	pShader->loadUniform("numLights", (int)world->getLightManager()->numPointLights());
	pShader->loadUniform("SunColor", world->getLightManager()->getSun()->getColor());
	pShader->loadUniform("SunDirection", *world->getLightManager()->getSun()->getDirection());
	pShader->loadUniform("viewmat", Camera::getActiveCamera()->getViewMatrix());
	pShader->loadUniform("viewPos", Camera::getActiveCamera()->getPosition());
	//pShader->loadUniform("viewDir", Camera::getActiveCamera()->getViewDirection());
	#ifndef GUM_ENGINE_NO_SHADOWMAP
	pShader->loadUniform("shadowMapMatrices", shadowmap->getMatrices());
    pShader->loadUniform("cascadeCount", (int)shadowmap->getCascadeLevels().size());
    pShader->loadUniform("cascadePlaneDistances", shadowmap->getCascadeLevels());
	#endif
    pShader->loadUniform("farPlane", (int)Settings::getSetting(Settings::RENDERDISTANCE));
	pShader->loadUniform("pixelSize", pixelSize);
	pShader->unuse();
    
	//auto elapsed = std::chrono::high_resolution_clock::now() - start;
	//microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

//
// Getter
//
long long Lightning::getExecutionTime()  { return microseconds; }

void Lightning::initShader()
{
  pShader = ShaderProgram::requestShaderProgram("LightningShader", true);
  pShader->addShader(Gum::PostProcessing::VertexShader);
  pShader->addShader(new Shader(LightningFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  pShader->build();

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
}

void Lightning::loadLight(Light* light, int index)
{
	if(index > GUM_MAX_LIGHTS)
	{
		Gum::Output::error("Maximum number of light exceeded");
		return;
	}

	pShader->use();
	pShader->loadUniform("lightPosition[" + std::to_string(index) + "]", light->getPosition());
	pShader->loadUniform("lightColor[" + std::to_string(index) + "]", light->getColor());

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


ShaderProgram* Lightning::getDefaultShaderProgram()
{
    return pShader;
}