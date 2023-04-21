#include "Skybox.h"
#include <System/MemoryManagement.h>
#include "SkyboxShaders.h"
#include "../Managers/ShaderManager.h"
#include "../Managers/TextureManager.h"
#include "System/Output.h"
#include <OpenGL/WrapperFunctions.h>

SkyBox::SkyBox(Mesh *mesh, vec3 *SunDirection, std::string name)
{
    pShader = nullptr;
    initShaders();
	this->sunDir = SunDirection;
	this->gradiant = true;
    this->v2Resolution = ivec2(1024);
    this->v2BRDFResolution = ivec2(512, 512);
    this->v2PreFilterResolution = ivec2(128, 128);

	//Create and add Properties
	pProperties->Name = name;
	pProperties->pMesh = mesh;
	load();
	getInstance(0)->setMatrix(Gum::Maths::createTransformationMatrix(this->getInstance(0)->getPosition(), this->getInstance(0)->getRotation(), this->getInstance(0)->getScale()));


	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    pFramebuffer = new Framebuffer(v2Resolution);
    pTexture = pFramebuffer->addCubeTextureAttachment(0, "Skybox", GL_RGBA, GL_RGBA32F, GL_FLOAT);
    pTexture->bind(0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    pTexture->unbind(0);

    pBRDFFramebuffer = new Framebuffer(v2BRDFResolution);
    pBRDFFramebuffer->addTextureAttachment(0, "SkyboxBRDFLUTMap", GL_RG, GL_RG16F, GL_FLOAT);
    pBRDFFramebuffer->getTextureAttachment(0)->bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    pBRDFFramebuffer->getTextureAttachment(0)->unbind(0);
    pBRDFCanvas = new Box(ivec2(0,0), v2BRDFResolution);

    pIrradianceFramebuffer = new Framebuffer(ivec2(32));
    pIrradianceFramebuffer->addCubeTextureAttachment(0, "SkyboxIrradiance", GL_RGBA, GL_RGBA32F, GL_FLOAT);
    pIrradianceFramebuffer->bind();
    unsigned int irradianceRBO = 0;
	glGenRenderbuffers(1, &irradianceRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, irradianceRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, pIrradianceFramebuffer->getSize().x, pIrradianceFramebuffer->getSize().y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, irradianceRBO);
    pIrradianceFramebuffer->unbind();


    pPreFilterMap = new Framebuffer(v2PreFilterResolution);
    pPreFilterMap->addCubeTextureAttachment(0, "SkyboxprefilterMap", GL_RGBA, GL_RGBA16F, GL_FLOAT);
    pPreFilterMap->bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glGenRenderbuffers(1, &captureRBOPreFiltered);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBOPreFiltered);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, pPreFilterMap->getSize().x, pPreFilterMap->getSize().y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBOPreFiltered);
    pPreFilterMap->unbind();


	captureProjection = Gum::Maths::perspective(90.0f, 1.0f, 0.1f, 1000.0f);
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f,   0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f, 1.0f,  0.0f), vec3(0.0f, 0.0f,  -1.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  -1.0f,  0.0f), vec3(0.0f, 0.0f,  1.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f)));
	captureViews.push_back(Gum::Maths::view(vec3(0.0f, 0.0f, 0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)));

    makeCubeMap(pTexture);
    updateTexture();
}

SkyBox::~SkyBox() 
{
	Gum::_delete(pFramebuffer);
    Gum::_delete(pIrradianceFramebuffer);
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
	prepareRender();
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
	pTexture->bind(0);
    getShader()->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
	getShader()->loadUniform("viewMatrix", Camera::ActiveCamera->getViewMatrix());
	getShader()->loadUniform("gradiant", (int)gradiant);
	getShader()->loadUniform("SunDirection", *this->sunDir);
	renderMesh();
	pTexture->unbind(0);

	finishRender();
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	if(isSpinning)
	{
		this->getInstance(0)->increaseRotation(vec3(0, FPS::get() * 50, 0));
	}
}

void SkyBox::update()
{
	Gum::Output::debug("Updating Skybox - empty");
}

void SkyBox::updateTexture()
{    
    pTexture->bind();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    pTexture->unbind();

	makeIrradianceMap();
	makePrefilterMap();
	makeBRDFMap();
}

void SkyBox::setTexture(Texture *tex)
{
	if(tex->getType() == Texture::TEXTUREHDR || tex->getType() == Texture::TEXTURE2D)
	{
		makeCubeMap(tex);
        pTexture = (TextureCube*)pFramebuffer->getTextureAttachment(0);
        pTexture->bind(0);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        pTexture->unbind();
        updateTexture();
	}
	else if(tex->getType() == Texture::TEXTURECUBE)
	{
		pTexture = (TextureCube*)tex;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BRDFMapShader->use();
	pBRDFCanvas->render();
	BRDFMapShader->unuse();
    pBRDFFramebuffer->unbind();
}

void SkyBox::makePrefilterMap()
{
	// convert HDR equirectangular environment map to cubemap equivalent
    pPreFilterMap->bind();
    PreFilteredMapShader->use();
    pTexture->bind(0);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = pPreFilterMap->getSize().x * std::pow(0.5, mip);
        unsigned int mipHeight = pPreFilterMap->getSize().y * std::pow(0.5, mip);
		
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBOPreFiltered);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        for (unsigned int i = 0; i < 6; ++i)
        {
            pPreFilterMap->drawAttachmentTexture(0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pPreFilterMap->getTextureAttachment(0)->getID(), mip);
            glClear(GL_COLOR_BUFFER_BIT);
			prepareRender();

        	PreFilteredMapShader->loadUniform("roughness", roughness);
            PreFilteredMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
            PreFilteredMapShader->loadUniform("projectionMatrix", captureProjection);
            PreFilteredMapShader->loadUniform("viewMatrix", captureViews[i]);

			glCullFace(GL_FRONT);
			glDisable(GL_DEPTH_TEST);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			renderMesh();
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
			finishRender();
        }
    }
    pTexture->unbind(0);
	PreFilteredMapShader->unuse();
    pPreFilterMap->unbind();
    
    pPreFilterMap->getTextureAttachment(0)->bind(0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    pPreFilterMap->getTextureAttachment(0)->unbind(0);
}

void SkyBox::makeIrradianceMap()
{
    pIrradianceFramebuffer->bind();
    IrradianceMapShader->use();
    pTexture->bind(0);
    
	for (unsigned int i = 0; i < 6; ++i)
	{
        pIrradianceFramebuffer->drawAttachmentTexture(0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pIrradianceFramebuffer->getTextureAttachment(0)->getID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareRender();

        IrradianceMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
	    IrradianceMapShader->loadUniform("projectionMatrix", captureProjection);
		IrradianceMapShader->loadUniform("viewMatrix", captureViews[i]);

		glCullFace(GL_FRONT);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderMesh();
		finishRender();
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
	}
    pTexture->unbind(0);
	IrradianceMapShader->unuse();
    pIrradianceFramebuffer->unbind();
}

void SkyBox::makeCubeMap(Texture* texture)
{
    pFramebuffer->bind();
    HDRToCubeMapShader->use();
    HDRToCubeMapShader->loadUniform("gradiant", (int)gradiant);
    HDRToCubeMapShader->loadUniform("SunDirection", *this->sunDir);
    texture->bind(0);
	for (unsigned int i = 0; i < 6; ++i)
	{
        //pFramebuffer->drawAttachmentTexture(0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pFramebuffer->getTextureAttachment()->getID(), 0);

		glClear(GL_COLOR_BUFFER_BIT);
		prepareRender();
		HDRToCubeMapShader->loadUniform("viewMatrix", captureViews[i]);
		HDRToCubeMapShader->loadUniform("projectionMatrix", captureProjection);
		HDRToCubeMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
        
		glCullFace(GL_FRONT);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderMesh();
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		finishRender();
	}
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    texture->unbind(0);
	HDRToCubeMapShader->unuse();
    pFramebuffer->unbind();
}

TextureCube* SkyBox::getTexture()              { return pTexture; }
TextureCube* SkyBox::getIrradianceMap()        { return (TextureCube*)pIrradianceFramebuffer->getTextureAttachment(0); }
TextureCube* SkyBox::getPreFilterMap()         { return (TextureCube*)pPreFilterMap->getTextureAttachment(0); }
Texture2D* SkyBox::getBRDFConvMap()            { return (Texture2D*)pBRDFFramebuffer->getTextureAttachment(0); }
void SkyBox::useGradiant(bool gradiant)        { this->gradiant = gradiant; updateTexture(); }
void SkyBox::spin(bool spin)                   { this->isSpinning = spin; }

void SkyBox::initShaders()
{
    if(pShader == nullptr)
    {
        Shader* skyVertexShader = new Shader(SkyboxVertexShader, Shader::TYPES::VERTEX_SHADER);

        pShader = new ShaderProgram();
        pShader->addShader(skyVertexShader);
        pShader->addShader(new Shader(SkyboxFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        pShader->build("SkyShader");
        pShader->addUniform("gradiant");
        pShader->addTexture("dayTexture", 0);
        pShader->addUniform("SunDirection");
        Gum::ShaderManager::addShaderProgram(pShader); // Make it public


        HDRToCubeMapShader = new ShaderProgram();
        HDRToCubeMapShader->addShader(skyVertexShader);
        HDRToCubeMapShader->addShader(new Shader(SkyboxHDRToCubeFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        HDRToCubeMapShader->build("HDRToCubeMapShader");
        HDRToCubeMapShader->addUniform("gradiant");
        HDRToCubeMapShader->addTexture("hdrTexture", 0);
        HDRToCubeMapShader->addUniform("SunDirection");

        
        IrradianceMapShader = new ShaderProgram();
        IrradianceMapShader->addShader(skyVertexShader);
        IrradianceMapShader->addShader(new Shader(SkyboxIrradianceFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        IrradianceMapShader->build("IrradianceMapShader");
        IrradianceMapShader->addUniform("cubeMap");

        
        PreFilteredMapShader = new ShaderProgram();
        PreFilteredMapShader->addShader(skyVertexShader);
        PreFilteredMapShader->addShader(new Shader(SkyboxPrefilterFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        PreFilteredMapShader->build("PreFilteredMapShader");
        PreFilteredMapShader->addUniform("roughness");
        PreFilteredMapShader->addUniform("cubeMap");

        
        BRDFMapShader = new ShaderProgram();
        BRDFMapShader->addShader(new Shader(SkyboxBRDFVertexShader, Shader::TYPES::VERTEX_SHADER));
        BRDFMapShader->addShader(new Shader(SkyboxBRDFFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
        BRDFMapShader->build("BRDFMapShader");
        BRDFMapShader->addUniform("roughness");
        BRDFMapShader->addUniform("cubeMap");
    }
}