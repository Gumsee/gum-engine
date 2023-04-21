#include "ShadowMapping.h"
#include "ShadowMappingShader.h"
#include "../../Managers/ShaderManager.h"


#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <OpenGL/WrapperFunctions.h>


ShadowMapping::ShadowMapping(Renderer3D* renderer)
{
	box = new ShadowBox(renderer);
    initShader();


    Framebuffer* framebuffer1 = new Framebuffer(ivec2(Settings::getSetting(Settings::Names::SHADOW_SIZE), Settings::getSetting(Settings::Names::SHADOW_SIZE)));
    //framebuffer1->addTextureAttachment(0, "ShadowMap1");
    framebuffer1->addDepthAttachment();
    framebuffer1->addDepthTextureAttachment();
    //framebuffer1->setDepthTextureAttachment(createDepthTextureAttachment(framebuffer1));
	vFramebuffers.push_back(framebuffer1);
	addShadowMap("ShadowMap", 0);

    Framebuffer* framebuffer2 = new Framebuffer(ivec2(Settings::getSetting(Settings::Names::SHADOW_SIZE), Settings::getSetting(Settings::Names::SHADOW_SIZE)));
    framebuffer2->addTextureAttachment(0, "ShadowMap2");
    framebuffer2->addDepthAttachment();
    framebuffer2->setDepthTextureAttachment(createDepthTextureAttachment(framebuffer2));
	vFramebuffers.push_back(framebuffer2);
	addShadowMap("ShadowMap2", 10);

    Framebuffer* framebuffer3 = new Framebuffer(ivec2(Settings::getSetting(Settings::Names::SHADOW_SIZE), Settings::getSetting(Settings::Names::SHADOW_SIZE)));
    framebuffer3->addTextureAttachment(0, "ShadowMap3");
    framebuffer3->addDepthAttachment();
    framebuffer3->setDepthTextureAttachment(createDepthTextureAttachment(framebuffer3));
	vFramebuffers.push_back(framebuffer3);
	addShadowMap("ShadowMap3", 100);

	Gum::Output::debug("ShadowMapping: Creating ShadowBox");
	box->create(&lightViewMatrix);

    
	this->m4BiasMatrix = mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);
}


ShadowMapping::~ShadowMapping() 
{
	Gum::_delete(box);
	for(int i = 0; i < vFramebuffers.size(); i++)
		Gum::_delete(vFramebuffers[i]);
}


void ShadowMapping::prepare(vec3 LightDirection, int index)
{
	if(index < (int)vFramebuffers.size())
	{
		// //Gum::Output::debug("Prepare ShadowMap");
		box->update();
		updateLightViewMatrix(LightDirection, box->getCenter());
		updateOrthoProjectionMatrix(box->getWidth(), box->getHeight(), box->getLength());
		//updateOrthoProjectionMatrix(1048, 1048, 1048);

		projectionViewMatrix = lightViewMatrix * projectionMatrix;
		vFramebuffers[index]->bind();
        glClearColor(1,1,1,1);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Gum::Output::debug(Settings::getSetting(Settings::Names::SHADOW_SIZE));

		vec3 lightInvDir = vec3(0.5f,2,2);
		
		//int size = 10;

		// Compute the MVP matrix from the light's point of view
		//mat4 depthProjectionMatrix = Math::ortho(-size,size,-size,size,-size,size *2); //TODO FIX ORDER
		//mat4 depthViewMatrix = Math::view(GumEngine::ActiveCamera->getPosition(), GumEngine::ActiveCamera->getPosition() - lightInvDir, vec3(0,1,0));
		//mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;
		
		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		// glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0])	
		//GetMatrixProduct[index] = createOffset() * projectionViewMatrix;

		//mat4 lightProjection, lightView;
		//vec3 lightPos(-2.0f, 4.0f, -1.0f);
        //float near_plane = 1.0f, far_plane = 7.5f;
        //lightProjection = perspective(radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        //lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
        //projectionViewMatrix = lightProjection * lightView;
		GetMatrixProduct[index] = /*m4BiasMatrix */  projectionViewMatrix;

		ShadowSize = Settings::getSetting(Settings::Names::SHADOW_SIZE);

		pShader->use();
        pShader->loadUniform("projectionMatrix", projectionMatrix);
        pShader->loadUniform("viewMatrix", lightViewMatrix);
		pShader->unuse();
	}
	glCullFace(GL_FRONT);
}

void ShadowMapping::finish()
{
	glCullFace(GL_BACK);
	vFramebuffers[0]->unbind();
}

void ShadowMapping::updateLightViewMatrix(vec3 direction, vec3 center)
{
	vec3 invertedDirection = vec3::normalize(direction);
	lightViewMatrix = Gum::Maths::view(center, center + invertedDirection, vec3(0, 1, 0));

	//vec3 vLightPos = -direction;
	//lightViewMatrix = lookAt(center + vLightPos, center + vec3(0, 0, 0), vec3(0, 1, 0));
}

/**
* Creates the orthographic projection matrix. This projection matrix
* basically sets the width, length and height of the "view cuboid", based
* on the values that were calculated in the {@link ShadowBox} class.
*
* @param width
*            - shadow box width.
* @param height
*            - shadow box height.
* @param length
*            - shadow box length.
*/
void ShadowMapping::updateOrthoProjectionMatrix(float width, float height, float length)
{
	projectionMatrix = mat4();
	projectionMatrix[0][0] = 2 / width;
	projectionMatrix[1][1] = 2 / height;
	projectionMatrix[2][2] = -2 / length;
	projectionMatrix[3][3] = 1;
}

Texture2D* ShadowMapping::createDepthTextureAttachment(Framebuffer* framebuffer)
{    
    framebuffer->bind();
    Texture2D* tex = new Texture2D("ShadowMapDepthTex");
    tex->bind();
    if(!gumTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, framebuffer->getSize(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0))
			Gum::Output::error("ShadowMapping::createDepthTextureAttachment: glTexImage Failed.");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Remove to debug
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->getID(), 0);
    tex->unbind();
    framebuffer->unbind();
    //framebuffer->setDepthTextureAttachment(tex);
    return tex;
}


void ShadowMapping::addShadowMap(std::string name, float offset)
{
	ShadowMapAmount++;
	textureOffsets.push_back(offset);
	GetMatrixProduct.push_back(mat4());
}

void ShadowMapping::initShader()
{
    if(Gum::ShaderManager::hasShaderProgram("ShadowMapShader"))
    {
	    pShader = Gum::ShaderManager::getShaderProgram("ShadowMapShader");
    }
    else
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(ShadowMappingVertexShader, Shader::VERTEX_SHADER));
        pShader->addShader(new Shader(ShadowMappingFragmentShader, Shader::FRAGMENT_SHADER));
        pShader->build("ShadowMapShader");
        pShader->addUniform("TextureMultiplier");
        pShader->addUniform("Displacement");
        pShader->addUniform("isInstanced");
        Gum::ShaderManager::addShaderProgram(pShader);
    }
}

//
// Getter
//
ShaderProgram* ShadowMapping::getShader() 			    { return pShader; }
mat4* ShadowMapping::getMatrix()				        { return &GetMatrixProduct[0]; }
int* ShadowMapping::getShadowMapSize() 				    { return &ShadowSize; }
Texture2D* ShadowMapping::getResultTexture(int Index)	{ return vFramebuffers[Index]->getDepthTextureAttachment(); }
