#pragma once
#include "Engine/General/World.h"
#include "Engine/Managers/TextureManager.h"
#include <gum-engine.h>

World* createGBufferExample()
{
    World* pWorld = new World();
	pWorld->getObjectManager()->getSkybox()->useGradiant(false); //PREFILTER MAP RENDERN BEI GRADIANT
	pWorld->getObjectManager()->getSkybox()->setTexture(Gum::TextureManager::getTexture("Sky/spaichingen_hill_8k.hdr", true));


	PointLight* pSpinningLight = new PointLight(vec3(10, 1, 0),   vec3(30), "light2");
    pWorld->getLightManager()->addPointLight(pSpinningLight);



	//Object *RoughBronzeObj = new Object("shaderBall.obj", "RoughBronze");
    Object *RoughBronzeObj = new Object(Mesh::generateCube(vec3(10)), "RoughBronze");
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/albedo.png"), 0);
	//RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RustedIron/normal.png"), 14);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/ao.png"), 7);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/roughness.png"), 8);
	RoughBronzeObj->getMaterial()->setTexture(Gum::TextureManager::getTexture("RustedIron/metallic.png"), 9);
	RoughBronzeObj->getMaterial()->setReflectivity(0);
	RoughBronzeObj->setPosition(vec3(0, -1, 0));
    RoughBronzeObj->getInstance()->renderOutline = true;
    pWorld->getObjectManager()->addObject(RoughBronzeObj);

    Object* floorPlane = new Object(Mesh::generatePlane(vec2(30,30)), "floorPlane");
	//floorPlane->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/diffuse.png"), 2);
	//floorPlane->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/ao.png"), 7);
	//floorPlane->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/roughness.png"), 8);
	//floorPlane->getMaterial()->setTexture(GumEngine::Textures->getTexture("Cobblestone/normal.png"), 14);
	floorPlane->getMaterial()->setTextureMultiplier(4);
	floorPlane->getMaterial()->setRoughness(0.5f);
	floorPlane->getMaterial()->setSpecularity(1);
    floorPlane->setPosition(vec3(0, -1, 0), 0);
    floorPlane->getProperties()->hasbackface = false;
    //GumEngine::DefaultRenderer->getObjectManager()->addObject(floorPlane);


    return pWorld;


    /*ShaderBuilder* pShaderBuilder = new ShaderBuilder("PostProcessingShader", Shader::VERTEX_SHADER, true);

    //Adding Default Attributes
    vec3* vertexPositionAttr      = (vec3*)pShaderBuilder->getPtrByName("vertexPosition");
    vec2* vertexTextureCoordsAttr = (vec2*)pShaderBuilder->getPtrByName("TextureCoords");
    vec3* vertexNormalsAttr       = (vec3*)pShaderBuilder->getPtrByName("Normals");

    //Adding Default Uniforms
    mat4* projectionMatrixUni     = (mat4*)pShaderBuilder->getPtrByName("projectionMatrix");
    mat4* viewMatrixUni           = (mat4*)pShaderBuilder->getPtrByName("viewMatrix");
    mat4* transformationMatrixUni = (mat4*)pShaderBuilder->getPtrByName("transformationMatrix");

    pShaderBuilder->addTo(vertexPositionAttr, projectionMatrixUni);

    pShaderBuilder->build();
    Gum::Output::info("\n"+pShaderBuilder->getShaderString());*/


    /*ShaderBuilder* pShaderBuilder = new ShaderBuilder("PostProcessingShaderVert", Shader::VERTEX_SHADER, false);
    vec3* vertexPositionAttr = new vec3(); pShaderBuilder->addAttribute(vertexPositionAttr, 0, "vertexPosition");
    vec2* texCoordVary = new vec2();       pShaderBuilder->addVarying(texCoordVary, "TexCoord");
    pShaderBuilder->set("gl_Position", pShaderBuilder->vec4(vertexPositionAttr, "1.0f"));
    pShaderBuilder->set(texCoordVary, pShaderBuilder->add(pShaderBuilder->elementOf(vertexPositionAttr, "xy"), "vec2(1.0f)"));
    pShaderBuilder->divTo(texCoordVary, "2.0f");



    ShaderBuilder* pFragShaderBuilder = new ShaderBuilder("PostProcessingShaderFrag", Shader::FRAGMENT_SHADER, false);
    vec3* fragColorAttr = new vec3(); pFragShaderBuilder->addAttribute(fragColorAttr, 0, "fragColor");
    pFragShaderBuilder->addVarying(texCoordVary, "TexCoord");
    ShaderTexture* textureSampler = new ShaderTexture(); pFragShaderBuilder->addTextureUniform(textureSampler, "texture0", "2D");
    pFragShaderBuilder->set(fragColorAttr, pFragShaderBuilder->texture2D(textureSampler, texCoordVary));


    ShaderProgram* pPostProcessingShader = new ShaderProgram();
    pPostProcessingShader->addShader(pShaderBuilder->build());
    pPostProcessingShader->addShader(pFragShaderBuilder->build());
    pPostProcessingShader->build("PostProcessingShader");*/
    //GumEngine::Shaders->addShader(pPostProcessingShader);

   // Gum::Output::info("\n"+pShaderBuilder->getShaderString());
    //Gum::Output::info("\n"+pFragShaderBuilder->getShaderString());
}