#include "Examples.h"
#include <Engine.h>
#include <GUI/Basics/Dropdown.h>
#include <GUI/Extra/FileExplorer/FileExplorer.h>
#include <GUI/XMLGUILoader.h>
#include <GUI/Basics/Graph.h>

/******

TODO
    Shader Editor (Nodes)
    Selecting Objects (Fix individual colors)
    Global ObjectManager
    Render ObjectIDs

 ******/

Examples::Examples()
{
    std::string example_path = EXAMPLES_PATH;

    GumEngine::initContextWindow("GBuffer Test", ivec2(90), true);
    GumEngine::initEssentials();
    GumEngine::initAssets(example_path+"/assets/");
	Input::Mouse->trap(false);
	Input::Mouse->hide(false);

    XMLGUILoader *pXMLGUILoader = new XMLGUILoader(example_path + "/assets/guis/example_interface.xml");
    GumEngine::GUIS->addGUI(pXMLGUILoader->getGUI());

    Box* pRenderCanvas = (Box*)pXMLGUILoader->getGUI()->findChildByID("renderview");
    //GumEngine::DefaultRenderer->setRenderCanvas(pRenderCanvas);
    GumEngine::initRenderer(pRenderCanvas);

    Settings::setSetting(Settings::SHOWDEBUGINFO, true);

    FileExplorer* pFileExplorer = new FileExplorer(ivec2(0,0), ivec2(100, 100), "./");
    pFileExplorer->setSizeInPercent(true, true);
    pXMLGUILoader->getGUI()->findChildByID("filesmenu")->addGUI(pFileExplorer);


    Tabs* TabsGUI = (Tabs*)pXMLGUILoader->getGUI()->findChildByID("sidemenutabs");
    TextBox* GBufferTab = TabsGUI->getTab("GBuffer");
    ((Box*)GBufferTab->findChildByID("Positions"))->setTexture(GumEngine::DefaultRenderer->getGBuffer()->getPositionMap());
    ((Box*)GBufferTab->findChildByID("Normals"))->setTexture(GumEngine::DefaultRenderer->getGBuffer()->getNormalMap());
    ((Box*)GBufferTab->findChildByID("Diffuse"))->setTexture(GumEngine::DefaultRenderer->getGBuffer()->getDiffuseMap());
    ((Box*)GBufferTab->findChildByID("IndividualColor"))->setTexture(GumEngine::DefaultRenderer->getGBuffer()->getIndividualColorMap());
    ((Box*)GBufferTab->findChildByID("SSAO"))->setTexture(GumEngine::DefaultRenderer->getSSAO()->getResultTexture());
    ((Box*)GBufferTab->findChildByID("Objectdata"))->setTexture(GumEngine::DefaultRenderer->getGBuffer()->getObjectDataMap());
    ((Box*)GBufferTab->findChildByID("ShadowMaps"))->setTexture(GumEngine::DefaultRenderer->getShadowMapping()->getResultTexture(0));

    float* fpsValue = new float(0);
    Graph* pFPSGraph = new Graph("FPS Graph", ivec2(0, 0), ivec2(600, 200), fpsValue);
    pFPSGraph->setColor(vec4(0.2, 0.6, 0.86, 1));
    pFPSGraph->setPrecision(0);
    GumEngine::GUIS->addGUI(pFPSGraph);

	GumEngine::ActiveCamera->setMode(GumEngine::ActiveCamera->THIRDPERSON);





    mWorlds["AnimatesModels"]           = nullptr;
    mWorlds["Billboards"]               = nullptr;
    mWorlds["GBuffer"]                  = nullptr;
    mWorlds["GUIs"]                     = nullptr;
    mWorlds["GumMaths"]                 = nullptr;
    mWorlds["IrradiancePBR"]            = nullptr;
    mWorlds["MazeLearning"]             = nullptr;
    mWorlds["Ocean"]                    = nullptr;
    mWorlds["Particles"]                = nullptr;
    mWorlds["PhysicallyBasedRendering"] = nullptr;
    mWorlds["Physics"]                  = nullptr;
    mWorlds["PostProcessing"]           = nullptr;
    mWorlds["ShadowMapping"]            = nullptr;
    mWorlds["SphereGenerator"]          = nullptr;

    TextBox* ExamplesTab = TabsGUI->getTab("Examples");
    for(auto example : mWorlds)
    {
        ((Dropdown*)ExamplesTab->findChildByID("examplesdropdown"))->addEntry(example.first, [example, this](std::string title) {
            GumEngine::DefaultRenderer->setWorld(getExampleWorld(example.first));
        });
    }

    GumEngine::DefaultRenderer->setWorld(getExampleWorld("Physics"));
    GumEngine::DefaultRenderer->setExposure(1.0f);
    


    float fTime = 0.0f;
    while(GumEngine::checkIsRunning())
    {
        if(pRenderCanvas->isMouseInside() && Input::Mouse->RightClick)
        {
            Input::Mouse->hide(true);
            Input::Mouse->trap(true);
            GumEngine::ActiveCamera->setMode(Camera::THIRDPERSON);
        }
        else 
        {
            Input::Mouse->hide(false);
            Input::Mouse->trap(false);
            GumEngine::ActiveCamera->setMode(Camera::STATIC);
        }

        GumEngine::update(GumEngine::ALL);
        //pSpinningLight->setPosition(vec3(std::cos(fTime) * 5, 4, std::sin(fTime) * 5));
        fTime += FPS::get() * 3.0f;
        *fpsValue = 1 / FPS::get();

        GumEngine::DefaultRenderer->getWorld()->getCamera()->setPosition(vec3(std::cos(fTime) * 15, 4, std::sin(fTime) * 15));

        //Input::Mouse->getDelta().print();

        GumEngine::prepareRender();
        GumEngine::render();
        GumEngine::renderGUI();
        GumEngine::finishRender();
    }
}

Examples::~Examples()
{

}


World* Examples::getExampleWorld(std::string name)
{
    if(mWorlds[name] == nullptr)
    {
        if(name == "AnimatesModels")            { mWorlds[name] = createAnimatedModelExample(); }
        if(name == "Billboards")                { mWorlds[name] = createBillboardsExample(); }
        if(name == "GBuffer")                   { mWorlds[name] = createGBufferExample(); }
        if(name == "GUIs")                      { mWorlds[name] = createGUIsExample(); }
        if(name == "GumMaths")                  { mWorlds[name] = createGumMathsExample(); }
        if(name == "IrradiancePBR")             { mWorlds[name] = createIrradiancePBRExample(); }
        if(name == "MazeLearning")              { mWorlds[name] = createMazeLearningExample(); }
        if(name == "Ocean")                     { mWorlds[name] = createOceanExample(); }
        if(name == "Particles")                 { mWorlds[name] = createParticlesExample(); }
        if(name == "PhysicallyBasedRendering")  { mWorlds[name] = createPhysicallyBasedRenderingExample(); }
        if(name == "Physics")                   { mWorlds[name] = createPhysicsExample(); }
        if(name == "PostProcessing")            { mWorlds[name] = createPostProcessingExample(); }
        if(name == "ShadowMapping")             { mWorlds[name] = createShadowMappingExample(); }
        if(name == "SphereGenerator")           { mWorlds[name] = createSphereGeneratorExample(); }
    }
    return mWorlds[name];
}


int main(int argc, char** argv)
{
    Examples examples;
    return 0;
}