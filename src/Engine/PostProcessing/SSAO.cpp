#include "SSAO.h"
#include "SSAOShader.h"

#include <System/MemoryManagement.h>
#include <OpenGL/WrapperFunctions.h>
#include "../Managers/ShaderManager.h"
#include "../General/Camera.h"
#include "../General/Renderer3D.h"

#include <random>
#include <chrono>
#include <iostream>

SSAO::SSAO(Box *gui, G_Buffer *gbuffer, Renderer3D* renderer) 
{
	this->pRenderer = renderer;
	this->rect = gui;
	this->gbuffer = gbuffer;
	this->pShader = nullptr;
	this->pBlurShader = nullptr;

	this->NoiseSize = 4;
	this->kernelSize = 8;
	this->power = 1.0f;// 2;
	this->radius = 0.75f;
	this->generatedNoise = false;
	this->generatedKernel = false;

    initShader();


    pSSAOFramebuffer = new Framebuffer(pRenderer->getRenderCanvas()->getSize());
    pSSAOFramebuffer->addTextureAttachment(0, "SSAOTexture", GL_RGBA, GL_RGBA16F, GL_FLOAT);

    
    pSSAOBlurFramebuffer = new Framebuffer(pRenderer->getRenderCanvas()->getSize());
    pSSAOBlurFramebuffer->addTextureAttachment(0, "SSAOBlurTexture", GL_RGBA, GL_RGBA16F, GL_FLOAT);

    pNoiseTexture = new Texture2D("SSAONoiseTexture");

	generateKernel();
}

SSAO::~SSAO() 
{
	Gum::_delete(pSSAOFramebuffer);
	Gum::_delete(pSSAOBlurFramebuffer);
	Gum::_delete(pNoiseTexture);

	pShader->removeShader(0);
	pBlurShader->removeShader(0);
	Gum::_delete(pShader);
	Gum::_delete(pBlurShader);
}


void SSAO::render()
{
	auto start = std::chrono::high_resolution_clock::now();
	noiseScale = vec2((float)pRenderer->getRenderCanvas()->getSize().x/(float)NoiseSize, (float)pRenderer->getRenderCanvas()->getSize().y/(float)NoiseSize); 


	pSSAOFramebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    

    gbuffer->getPositionMap()->bind(0);
	gbuffer->getNormalMap()->bind(1);

	if(pNoiseTexture != nullptr)
		pNoiseTexture->bind(2);

	//gbuffer->getDepthMap()->bind(3);


	pShader->use();
    pShader->loadUniform("projectionMatrix", Camera::ActiveCamera->getProjectionMatrix());
	pShader->loadUniform("noiseScale", noiseScale);
	pShader->loadUniform("kernelSize", (int)kernelSize);
	pShader->loadUniform("radius", (float)radius);
	pShader->loadUniform("far", (float)Settings::getSetting(Settings::RENDERDISTANCE));
	pShader->loadUniform("power", (float)power);

	rect->render();
	pShader->unuse();

	pSSAOBlurFramebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    pSSAOFramebuffer->getTextureAttachment(0)->bind(0);

	pBlurShader->use();
    pBlurShader->loadUniform("NoiseSize", (int)NoiseSize);
	rect->render();
	pBlurShader->unuse();

    pSSAOFramebuffer->getTextureAttachment(0)->unbind(0);
	pSSAOBlurFramebuffer->unbind();
	
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

void SSAO::generateKernel()
{
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator(lSeed);
	//srand(time(0));

	if(!generatedKernel)
	{
		ssaoKernel.clear();
		for (unsigned int i = 0; i < kernelSize; ++i)
		{
			vec3 sample(
				randomFloats(generator) * 2.0 - 1.0, 
				randomFloats(generator) * 2.0 - 1.0, 
				randomFloats(generator)
			);
			
			sample  = vec3::normalize(sample);
			sample *= randomFloats(generator);

			float scale = (float)i / (float)kernelSize;
			scale   = Gum::Maths::lerp(scale * scale, 0.1f, 1.0f);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}
		generatedKernel = true;
	}

    ssaoNoise.clear();
    for (unsigned int i = 0; i < NoiseSize * NoiseSize; i++)
    {
        ssaoNoise.push_back(randomFloats(generator) * 2.0 - 1.0);
        ssaoNoise.push_back(randomFloats(generator) * 2.0 - 1.0);
    }  
    pNoiseTexture->setSize(ivec2(NoiseSize, NoiseSize));
    pNoiseTexture->bind(0);
    if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, ivec2(NoiseSize), 0, GL_RG, GL_FLOAT, &ssaoNoise[0]))
			Gum::Output::error("SSAO::generateKernel: glTexImage Failed.");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    pNoiseTexture->unbind(0);

    
    pShader->use();
    for (unsigned int i = 0; i < kernelSize; ++i)
    {
        pShader->loadUniform("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    }
    pShader->unuse();
}

void SSAO::setKernelSize(const unsigned int& kernelsize)
{
	if(kernelsize != this->kernelSize)
	{
		this->kernelSize = kernelsize;
		generatedKernel = false;
		generateKernel();
	}
}

void SSAO::setNoiseSize(const unsigned int& noisesize)
{
	unsigned int nssize = noisesize;
	if(nssize % 2 != 0)
		nssize++;

	if(nssize != this->NoiseSize)
	{
		this->NoiseSize = nssize;
		generatedNoise = false;
		generateKernel();	
	}
}

void SSAO::setPower(const float& power) 	{ this->power = power; }
void SSAO::setRadius(const float& radius) 	{ this->radius = radius; }

float SSAO::getPower() const				{ return power; }
int SSAO::getKernelSize() const			    { return kernelSize; }
int SSAO::getNoiseSize() const			    { return NoiseSize; }
float SSAO::getRadius() const			    { return radius; }
long long SSAO::getExecutionTime() const	{ return microseconds; }
Texture2D* SSAO::getResultTexture()         { return (Texture2D*)pSSAOBlurFramebuffer->getTextureAttachment(0); }
Texture2D* SSAO::getNoiseTexture() 	        { return pNoiseTexture; }



/*ShaderBuilder* vbuilder = new ShaderBuilder("SSAOShaderFrag", Shader::FRAGMENT_SHADER);
vec2* texCoordsVary     = vbuilder->addVarying(new vec2(), ShaderBuilder::Defaults::TEXCOODS_VARY_NAME);

ShaderTexture* positionTex = vbuilder->addTextureUniform(new ShaderTexture(), "positionTex", "2D");
ShaderTexture* normalTex   = vbuilder->addTextureUniform(new ShaderTexture(), "normalTex", "2D");
ShaderTexture* noiseTex    = vbuilder->addTextureUniform(new ShaderTexture(), "noiseTex", "2D");

float* powerUni        = vbuilder->addUniform(new float(), "fPower");
int* kernelSizeUni     = vbuilder->addUniform(new int(),   "iKernelSize");
float* radiusUni       = vbuilder->addUniform(new float(), "fRadius");
vec2* noiseScaleUni    = vbuilder->addUniform(new vec2(),  "fNoiseScale");
mat4* projectionMatUni = vbuilder->addUniform(new mat4(),  "m4ProjectionMat");

float* biasConst = vbuilder->addConstant(new float(0.025f), "bias");

vec3* fragPos    = vbuilder->var(new vec3(), "fragPos");
vec3* fragNormal = vbuilder->var(new vec3(), "fragNormal");
vec3* randomVec  = vbuilder->var(new vec3(), "randomVec");

vbuilder->set(fragPos, vbuilder->elementOf(vbuilder->texture2D(positionTex, texCoordsVary), "rgb"));
vbuilder->set(fragNormal, vbuilder->normalize(vbuilder->elementOf(vbuilder->texture2D(normalTex, texCoordsVary), "rgb")));
vbuilder->set(randomVec, vbuilder->normalize(vbuilder->elementOf(vbuilder->texture2D(normalTex, texCoordsVary), "rgb")));*/

void SSAO::initShader()
{
    if(pShader == nullptr)
	{
		pShader = new ShaderProgram();
        pShader->addShader(Gum::ShaderManager::getShader("PostProcessingShaderVert"));
        pShader->addShader(new Shader(SSAOFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("SSAOShader");
        pShader->addTexture("texPosition", 0);
        pShader->addTexture("texNormal", 1);
        pShader->addTexture("texNoise", 2);
        pShader->addUniform("projectionMatrix");
        pShader->addUniform("viewMatrix");
        pShader->addUniform("kernelSize");
        pShader->addUniform("radius");
        pShader->addUniform("power");
        pShader->addUniform("noiseScale");
        for (unsigned int i = 0; i < kernelSize; ++i)
        {
            pShader->addUniform("samples[" + std::to_string(i) + "]");
        }
    }

    if(pBlurShader == nullptr)
    {
        pBlurShader = new ShaderProgram();
        pBlurShader->addShader(Gum::ShaderManager::getShader("PostProcessingShaderVert"));
        pBlurShader->addShader(new Shader(SSAOBlurFragmentShader, Shader::FRAGMENT_SHADER));
        pBlurShader->build("SSAOBlurShader");
        pBlurShader->addTexture("ssaoInput", 0);
        pBlurShader->addUniform("NoiseSize");
    }
}