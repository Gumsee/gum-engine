#pragma once
#include <Engine/3D/World3D.h>
#include <Engine/3D/Object/SceneObject.h>
#include <gum-engine.h>

World3D* createGBufferExample(Gum::File assetPath)
{
    World3D* pWorld3D = new World3D();
	pWorld3D->getObjectManager()->getSkybox()->renderSky(false); //PREFILTER MAP RENDERN BEI GRADIANT
	pWorld3D->getObjectManager()->getSkybox()->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/sky/evening_meadow_8k.hdr"), true));
  //pWorld3D->getObjectManager()->getSkybox()->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/sky/moonless_golf_8k.hdr"), true));

  //std::cout << "datatype: " << Texture::autoLoad(assetPath + Gum::File("textures/sky/spaichingen_hill_8k.hdr"), true)->getType() << " " << Texture::TEXTUREHDR << std::endl;


	PointLight* pSpinningLight = new PointLight(vec3(10, 1, 0),   vec3(30), "light2");
  //pWorld3D->getLightManager()->addPointLight(pSpinningLight);



	//Object *RoughBronzeObj = new Object("shaderBall.obj", "RoughBronze");
    //SceneObject *RoughBronzeObj = new SceneObject(Mesh::generateSphere(10, 32, 17), "RoughBronze");
    SceneObject *RoughBronzeObj = new SceneObject(assetPath + Gum::File("objects/shaderball.obj"), "RoughBronze");
    RoughBronzeObj->addInstance();
    Material* roughBronzeMaterial = new Material();
	  roughBronzeMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/RustedIron/albedo.png")), 0);
	  roughBronzeMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/RustedIron/normal.png")), 14);
	  roughBronzeMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/RustedIron/ao.png")), 7);
	  roughBronzeMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/RustedIron/roughness.png")), 8);
	  roughBronzeMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/RustedIron/metallic.png")), 9);
    //roughBronzeMaterial->setColor(rgba(255,128,0,255));
	  //roughBronzeMaterial->setReflectivity(.01);
    RoughBronzeObj->setMaterial(roughBronzeMaterial);
	  RoughBronzeObj->getInstance()->setPosition(vec3(0, -11, 0));
    RoughBronzeObj->getInstance()->setScale(vec3(6));
    //RoughBronzeObj->getInstance()->renderOutline = true;
    pWorld3D->getObjectManager()->addObject(RoughBronzeObj);

    SceneObject* floorPlane = new SceneObject(Mesh::generatePlane(vec2(30,30)), "floorPlane");
    floorPlane->addInstance();
    Material* floorPlaneMaterial = new Material();
	  floorPlaneMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/Cobblestone/diffuse.png")), 0);
	  floorPlaneMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/Cobblestone/ao.png")), 7);
	  floorPlaneMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/Cobblestone/roughness.png")), 8);
	  floorPlaneMaterial->setTexture(Texture::autoLoad(assetPath + Gum::File("textures/Cobblestone/normal.png")), 14);
	  floorPlaneMaterial->setTextureMultiplier(4);
	  floorPlaneMaterial->setRoughness(0.5f);
	  floorPlaneMaterial->setSpecularity(.1);
    floorPlane->setMaterial(floorPlaneMaterial);
    floorPlane->getInstance()->setPosition(vec3(0, -11, 0));
    floorPlane->getInstance()->setScale(vec3(1, 1, 1));
    //floorPlane->getProperties()->hasbackface = false;
    pWorld3D->getObjectManager()->addObject(floorPlane);


    return pWorld3D;


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


    ShaderProgram* pPostProcessingShader = new ShaderProgram(true);
    pPostProcessingShader->addShader(pShaderBuilder->build());
    pPostProcessingShader->addShader(pFragShaderBuilder->build());
    pPostProcessingShader->build("PostProcessingShader");*/
    //GumEngine::Shaders->addShader(pPostProcessingShader);

   // Gum::Output::info("\n"+pShaderBuilder->getShaderString());
    //Gum::Output::info("\n"+pFragShaderBuilder->getShaderString());
}