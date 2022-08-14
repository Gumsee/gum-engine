#include <Engine.h>
#include <GUI/Basics/Tabs.h>
#include <PostProcessing/Effects/GaussianBlur.h>
#include <PostProcessing/Effects/Brightfilter.h>
#include "../Examples.h"

World* Examples::createPostProcessingExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

	pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));

    GUIWindow* previewWindow = new GUIWindow(ivec2(0,0), ivec2(800, 600), "Effects", true);
    Tabs* pTabs = new Tabs(ivec2(0,0), ivec2(100, 100), ivec2(100, 30));
    pTabs->setSizeInPercent(true, true);
    pTabs->addTab("Positions", true);
    previewWindow->addGUI(pTabs);

    GumEngine::GUIS->addWindow(previewWindow);


    //EFFECTS TO ADD
	/*Framebuffer* pFramebuffer;
	GaussianBlur* pGaussianBlur;
	brightfilter* pBrightfilter;
	combineTextures* pTextureCombine;
	ContrastChanger* pContrastchanger;*/


	Object *RoughBronzeObj = new Object("shaderBall.obj", "RoughBronze");
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/albedo.png"), 2);
	//RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/normal.png"), 14);
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/roughness.png"), 8);
	RoughBronzeObj->getMaterial()->setTexture(GumEngine::Textures->getTexture("RoughBronze/metallic.png"), 9);
	RoughBronzeObj->getMaterial()->setReflectivity(0);
	RoughBronzeObj->setPosition(vec3(0, -1, 0));
    pWorld->getObjectManager()->addObject(RoughBronzeObj);

    //GaussianBlur* pGaussianBlur = new GaussianBlur(pWorld->getRenderCanvas(), Gum::Window->getRenderQuadSize(), 1);
    Brightfilter* pBrightfilter = new Brightfilter(GumEngine::DefaultRenderer->getRenderCanvas(), Gum::Window->getRenderQuadSize());
    GumEngine::DefaultRenderer->addPostProcessingEffect(pBrightfilter);

	return pWorld;
}