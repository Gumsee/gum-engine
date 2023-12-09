#include "Skybox.h"
#include <System/MemoryManagement.h>
#include "Graphics/Framebuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureCube.h"
#include "Graphics/Variables.h"
#include "SkyboxShaders.h"
#include "../../Shaders/ShaderManager.h"
#include "../../Texture/TextureManager.h"
#include "System/Output.h"
#include <Graphics/WrapperFunctions.h>

SkyBox::SkyBox(Mesh *mesh, vec3 *SunDirection, std::string name)
{
    pShader = nullptr;
    initShaders();
	this->sunDir = SunDirection;
	this->gradiant = true;

	//Create and add Properties
	sName = name;
	pMesh = mesh;
	load();
    addInstance();

    pTexture = new EnvironmentMap(ivec2(1024, 1024), "Skybox", Gum::Graphics::Datatypes::FLOAT);
    pIrradianceMap = new EnvironmentMap(ivec2(32, 32), "IrradianceMap", Gum::Graphics::Datatypes::Datatypes::FLOAT);
    pPreFilterMap = new EnvironmentMap(ivec2(128, 128), "PreFilterMap", Gum::Graphics::Datatypes::Datatypes::FLOAT);
    pPreFilterMap->createMipmaps();
    pPreFilterMap->clampToEdge();
    pPreFilterMap->setFiltering(Texture::FilteringTypes::LINEAR_MIPMAP_LINEAR);

    pBRDFFramebuffer = new Framebuffer(ivec2(512, 512));
    pBRDFFramebuffer->addTextureAttachment(0, "SkyboxBRDFLUTMap", Gum::Graphics::Datatypes::Datatypes::FLOAT, 2U);
    pBRDFFramebuffer->getTextureAttachment(0)->clampToEdge();
    pBRDFFramebuffer->getTextureAttachment(0)->setFiltering(Texture::FilteringTypes::LINEAR);
    pBRDFCanvas = new Box(ivec2(0,0), pBRDFFramebuffer->getSize());

    makeCubeMap(nullptr);
    updateTexture();
}

SkyBox::~SkyBox() 
{
	Gum::_delete(pTexture);
    Gum::_delete(pIrradianceMap);
	Gum::_delete(pBRDFFramebuffer);
	Gum::_delete(pPreFilterMap);
	Gum::_delete(pBRDFCanvas);

    
    HDRToCubeMapShader->removeShader(0);
    IrradianceMapShader->removeShader(0);
    PreFilteredMapShader->removeShader(0);
	Gum::_delete(HDRToCubeMapShader);
	Gum::_delete(IrradianceMapShader);
	Gum::_delete(PreFilteredMapShader);
	Gum::_delete(BRDFMapShader);
}

void SkyBox::render()
{
    Gum::Graphics::cullBackside(false);
    Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    Gum::Graphics::renderWireframe(false);
    
	pTexture->bind(0);
    //pPreFilterMap->bind(); //TODO
    getShaderProgram()->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
    getShaderProgram()->loadUniform("projectionMatrix", Camera::getActiveCamera()->getPerspective());
	getShaderProgram()->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
	getShaderProgram()->loadUniform("gradiant", (int)gradiant);
	getShaderProgram()->loadUniform("SunDirection", *this->sunDir);
	renderMesh();
	pTexture->unbind(0);

    Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    Gum::Graphics::cullBackside(true);
	if(isSpinning)
		this->getInstance(0)->increaseRotation(vec3(0, FPS::get() * 50, 0));
}

void SkyBox::updateTexture()
{
    Gum::Graphics::cullBackside(false);
    Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    Gum::Graphics::renderWireframe(false);
	makeIrradianceMap();
	makePrefilterMap();
	makeBRDFMap();
    Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    Gum::Graphics::cullBackside(true);
}

void SkyBox::setTexture(Texture *tex)
{
	if(tex->getType() == Texture::TEXTUREHDR || tex->getType() == Texture::TEXTURE2D)
	{
		makeCubeMap(tex);
        pTexture->createMipmaps();
        updateTexture();
	}
	else if(tex->getType() == Texture::TEXTURECUBE)
	{
		Gum::Output::warn("Skybox: Texturecube not implemented yet!");
		//pTexture = (TextureCube*)tex;
		updateTexture();
	}
	else
	{
		Gum::Output::error("Skybox: invalid Texture set!");
	}
}

void SkyBox::makeBRDFMap()
{
	pBRDFFramebuffer->bind();
    pBRDFFramebuffer->clear(Framebuffer::ClearFlags::COLOR);
	BRDFMapShader->use();
	pBRDFCanvas->render();
	BRDFMapShader->unuse();
    pBRDFFramebuffer->unbind();
}

void SkyBox::makePrefilterMap()
{
    ivec2 res = pPreFilterMap->getSize();
    PreFilteredMapShader->use();
    pTexture->bind(0);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // resize framebuffer according to mip-level size.
        pPreFilterMap->setActiveMipmapLevel(mip);
        pPreFilterMap->getFramebuffer()->setSize(vec2(res) * std::pow(0.5, mip));

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        pPreFilterMap->render([this, roughness]() {
        	PreFilteredMapShader->loadUniform("roughness", roughness);
            PreFilteredMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
            PreFilteredMapShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
            PreFilteredMapShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());

			renderMesh();
        });
    }
    pTexture->unbind(0);
	PreFilteredMapShader->unuse();

    pPreFilterMap->setActiveMipmapLevel(0);
    pPreFilterMap->getFramebuffer()->setSize(res);
}

void SkyBox::makeIrradianceMap()
{
    pTexture->bind(0);
    IrradianceMapShader->use();
    IrradianceMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
    pIrradianceMap->render([this]() {
        IrradianceMapShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
        IrradianceMapShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
        renderMesh();
    });
	IrradianceMapShader->unuse();
    pTexture->unbind(0);
}

void SkyBox::makeCubeMap(Texture* texture)
{
    Gum::Graphics::cullBackside(false);
    Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    Gum::Graphics::renderWireframe(false);
    if(texture != nullptr)
        texture->bind(0);

    HDRToCubeMapShader->use();
    HDRToCubeMapShader->loadUniform("gradiant", (int)gradiant);
    HDRToCubeMapShader->loadUniform("SunDirection", *this->sunDir);
    pTexture->render([this]() {
        HDRToCubeMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
        HDRToCubeMapShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
        HDRToCubeMapShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
        renderMesh();
    });
	HDRToCubeMapShader->unuse();
    TextureCube::unbindGlobal(0);
    Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
    Gum::Graphics::cullBackside(true);
}


//
// Getter
//
TextureCube* SkyBox::getTexture()              { return pTexture; }
TextureCube* SkyBox::getIrradianceMap()        { return pIrradianceMap; }
TextureCube* SkyBox::getPreFilterMap()         { return pPreFilterMap; }
Texture2D* SkyBox::getBRDFConvMap()            { return (Texture2D*)pBRDFFramebuffer->getTextureAttachment(0); }


//
// Setter
//
void SkyBox::useGradiant(bool gradiant)        { this->gradiant = gradiant; updateTexture(); }
void SkyBox::spin(bool spin)                   { this->isSpinning = spin; }



void SkyBox::initShaders()
{
    if(pShader == nullptr)
    {
        Shader* skyVertexShader = new Shader(SkyboxVertexShader, Shader::TYPES::VERTEX_SHADER);

        pShader = new ShaderProgram(true);
        pShader->addShader(skyVertexShader);
        pShader->addShader(new Shader(SkyboxFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("SkyShader");
        pShader->addUniform("gradiant");
        pShader->addTexture("dayTexture", 0);
        pShader->addUniform("SunDirection");
        Gum::ShaderManager::addShaderProgram(pShader); // Make it public


        HDRToCubeMapShader = new ShaderProgram(true);
        HDRToCubeMapShader->addShader(skyVertexShader);
        HDRToCubeMapShader->addShader(new Shader(SkyboxHDRToCubeFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        HDRToCubeMapShader->build("HDRToCubeMapShader");
        HDRToCubeMapShader->addUniform("gradiant");
        HDRToCubeMapShader->addTexture("hdrTexture", 0);
        HDRToCubeMapShader->addUniform("SunDirection");

        
        IrradianceMapShader = new ShaderProgram(true);
        IrradianceMapShader->addShader(skyVertexShader);
        IrradianceMapShader->addShader(new Shader(SkyboxIrradianceFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        IrradianceMapShader->build("IrradianceMapShader");
        IrradianceMapShader->addTexture("cubeMap", 0);

        
        PreFilteredMapShader = new ShaderProgram(true);
        PreFilteredMapShader->addShader(skyVertexShader);
        PreFilteredMapShader->addShader(new Shader(SkyboxPrefilterFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        PreFilteredMapShader->build("PreFilteredMapShader");
        PreFilteredMapShader->addUniform("roughness");
        PreFilteredMapShader->addTexture("cubeMap", 0);

        
        BRDFMapShader = new ShaderProgram(true);
        BRDFMapShader->addShader(new Shader(SkyboxBRDFVertexShader, Shader::TYPES::VERTEX_SHADER));
        BRDFMapShader->addShader(new Shader(SkyboxBRDFFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        BRDFMapShader->build("BRDFMapShader");
        BRDFMapShader->addUniform("roughness");
        BRDFMapShader->addTexture("cubeMap", 0);
    }
}