#include "G_Buffer.h"
#include "GBufferShader.h"
#include "../Managers/ShaderManager.h"

/*
    0 PositionMap;
    1 IndividualColorMap
    2 NormalMap;
    3 DiffuseMap;
    4 ObjectDataMap;
*/
G_Buffer::G_Buffer(ivec2 resolution)
{
    gBuffer = new Framebuffer(resolution);
    gBuffer->addTextureAttachment(0, "G_BufferPositionMap",   GL_RGBA, GL_RGBA32F, GL_FLOAT);
    gBuffer->addTextureAttachment(1, "G_BufferIndividualMap", GL_RGBA, GL_RGBA);
    gBuffer->addTextureAttachment(2, "G_BufferNormalMap",     GL_RGBA, GL_RGBA32F, GL_FLOAT);
    gBuffer->addTextureAttachment(3, "G_BufferDiffuseMap",    GL_RGBA, GL_RGBA);
    gBuffer->addTextureAttachment(4, "G_BufferObjectDataMap", GL_RGBA, GL_RGBA);

    gBuffer->addDepthAttachment();
    gBuffer->addDepthTextureAttachment();
    //gBuffer->addDepthStencilTextureAttachment("GBufferDepthTextureAttachment");

    initShader();
}

void G_Buffer::bind()
{
    start = std::chrono::high_resolution_clock::now();
    gBuffer->bind();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_COLOR);  
}

void G_Buffer::unbind()
{
    //glDisable(GL_BLEND);
    gBuffer->unbind();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}


//
// Getter
//
int G_Buffer::getDepthBuffer()              { return gBuffer->getDepthAttachmentID(); }
ShaderProgram *G_Buffer::getShader()          { return this->pShader; }
Texture* G_Buffer::getPositionMap()         { return gBuffer->getTextureAttachment(0); }
Texture* G_Buffer::getIndividualColorMap()  { return gBuffer->getTextureAttachment(1); }
Texture* G_Buffer::getNormalMap()           { return gBuffer->getTextureAttachment(2); }
Texture* G_Buffer::getDiffuseMap()          { return gBuffer->getTextureAttachment(3); }
Texture* G_Buffer::getObjectDataMap()       { return gBuffer->getTextureAttachment(4); }
Texture* G_Buffer::getDepthMap()            { return gBuffer->getDepthTextureAttachment(); }
long long G_Buffer::getExecutionTime()      { return microseconds; }
Framebuffer* G_Buffer::getFramebuffer()     { return gBuffer; }


void G_Buffer::initShader()
{
    if(Gum::ShaderManager::hasShaderProgram("GBufferShader"))
    {
        pShader = Gum::ShaderManager::getShaderProgram("GBufferShader");
    }
    else
    {
        pShader = new ShaderProgram();
        pShader->addShader(new Shader(GBufferVertexShader, Shader::VERTEX_SHADER));
        pShader->addShader(new Shader(GBufferFragmentShader, Shader::FRAGMENT_SHADER));

        pShader->build("GBufferShader");
        pShader->addUniform("color");
        pShader->addUniform("viewPos");
        pShader->addUniform("TextureMultiplier");
        pShader->addUniform("transformationMatrix");
        for(int i = 0; i < 100; i++)
            pShader->addUniform("gBones[" + std::to_string(i) + "]");
        
        pShader->addUniform("hasNormalMap");
        pShader->addUniform("hasSpecularMap");
        pShader->addUniform("hasReflectionMap");
        pShader->addUniform("hasRefractionMap");
        pShader->addUniform("hasRoughnessMap");
        pShader->addUniform("hasDisplacementMap");
        pShader->addUniform("hasAmbientOcclusionMap");
        pShader->addUniform("hasTexture");

        pShader->addUniform("inverseCulling");
        pShader->addUniform("ReflectionFactor");
        pShader->addUniform("RefractionFactor");
        pShader->addUniform("specularFactor");
        pShader->addUniform("roughnessFactor");
        pShader->addUniform("ClipPlane");
        pShader->addUniform("flipNormal");
        pShader->addUniform("isInstanced");
        pShader->addUniform("isSkeletal");


        pShader->addTexture("texture0", 0);
        pShader->addTexture("ambientOcclusionmap", 7);
        pShader->addTexture("roughnessmap", 8);
        pShader->addTexture("specularmap", 9);
        pShader->addTexture("refractionmap", 10);
        pShader->addTexture("reflectionmap", 11);
        pShader->addTexture("Displacement", 13);
        pShader->addTexture("normalmap", 14);
        pShader->addTexture("Enviorment", 15);
        pShader->addTexture("ShadowMap", 16);
        Gum::ShaderManager::addShaderProgram(pShader);
    }
}