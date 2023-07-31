#include <Engine.h>
#include <Globals.h>
#include <Extras/Ocean.h>
#include "../Examples.h"

World3D* Examples::createOceanExample()
{
    World3D* pWorld3D = new World3D(Gum::Window->getSize());
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(false); //PREFILTER MAP RENDERN BEI GRADIANT
	pWorld3D->getObjectManager()->getSkybox()->setTexture(GumEngine::Textures->getTexture("Sky/spaichingen_hill_8k.hdr", true));

	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));

    Ocean *pOcean = new Ocean("TestOcean");

    ShaderProgram* pOceanShader = new ShaderProgram();
    pOceanShader->addShader(new Shader(Tools::readFileContents(GumGlobals::SHADER_ASSETS_PATH + "ocean.vert"), Shader::TYPES::VERTEX_SHADER));
    pOceanShader->addShader(new Shader(Tools::readFileContents(GumGlobals::SHADER_ASSETS_PATH + "ocean.frag"), Shader::TYPES::FRAGMENT_SHADER));
    pOceanShader->build("OceanShader");
    pOceanShader->addUniform("fTime");
    pOceanShader->addUniform("v3ViewDir");
    pOceanShader->addUniform("v3ViewPos");
    pOceanShader->addTexture("texture0", 0);
    GumEngine::Shaders->addShaderProgram(pOceanShader);

    Object* planeObj = new Object(Mesh::generatePlane(vec2(10,10)), "plane");
    planeObj->setShader(pOceanShader);
    planeObj->getProperties()->hasbackface = false;
    planeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("particleAtlas.png"), 0);
    pWorld3D->getObjectManager()->addObject(planeObj);

    float fTime = 0.0f;
    pWorld3D->addUpdateable([fTime, pOceanShader]() mutable {
        fTime += FPS::get() * 10.0f;
        pOceanShader->use();
        pOceanShader->loadUniform("fTime", fTime);
        pOceanShader->loadUniform("v3ViewDir", GumEngine::ActiveCamera->getViewDirection());
        pOceanShader->loadUniform("v3ViewPos", GumEngine::ActiveCamera->getPosition());
       // GumEngine::ActiveCamera->getViewDirection().print();
        pOceanShader->unuse();
    });

	return pWorld3D;
}