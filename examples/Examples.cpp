#include "Examples.h"
#include <GUI/Basics/Dropdown.h>
//#include <GUI/Extra/FileExplorer/FileExplorer.h>
//#include <GUI/XMLGUILoader.h>
#include <GUI/Basics/Graph.h>
#include "Desktop/IO/Mouse.h"
#include "Engine/General/Camera.h"
#include "Engine/General/Renderer3D.h"
#include "Essentials/Tools.h"
#include "GBufferTest.h"
#include "BasicCube.h"
#include "OpenGL/Framebuffer.h"
#include "OpenGL/Texture2D.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"
#include <OpenGL/OpenGL.h>

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

    Gum::Display::init();
    pMainWindow = new Gum::Window("GUI Example", ivec2(75, 75), GUM_WINDOW_SIZE_IN_PERCENT | GUM_WINDOW_RESIZABLE);
    pMainWindow->setClearColor(vec4(0.24f, 0.39f, 0.75f, 1.0f));
    pMainWindow->setVerticalSync(true);
	pMainWindow->getMouse()->trap(false);
	pMainWindow->getMouse()->hide(false);
    Gum::OpenGL::addFramebufferToWindow(pMainWindow);

    pGUI = new Gum::GUI(pMainWindow);

    //ObjectManager::MODEL_ASSETS_PATH = example_path + "/assets/";
    Gum::Engine::init();

    XMLLoader pXMLGUILoader(example_path + "/assets/guis/example_interface.xml");
    pGUI->addGUI(pXMLGUILoader.getRootGUI());
    pRenderCanvas = (Box*)pXMLGUILoader.getRootGUI()->findChildByID("renderview");

    Settings::setSetting(Settings::SHOWDEBUGINFO, true);

    pMainCamera = new Camera(pMainWindow->getSize(), new World());
    pMainCamera->setPosition(vec3(0,0,0));
    pMainCamera->setMode(Camera::Modes::THIRDPERSON);
    pMainCamera->setOffset(60.0f);

    pMainRenderer = new Renderer3D(pRenderCanvas, pMainWindow);
    pMainWindow->onResized([this](ivec2 size) {
        pGUI->setSize(size);
        pMainRenderer->updateFramebufferSize();
    });



    /*FileExplorer* pFileExplorer = new FileExplorer(ivec2(0,0), ivec2(100, 100), "./");
    pFileExplorer->setSizeInPercent(true, true);
    pXMLGUILoader->getGUI()->findChildByID("filesmenu")->addGUI(pFileExplorer);*/


    /*Tabs* TabsGUI = (Tabs*)pXMLGUILoader->getGUI()->findChildByID("sidemenutabs");
    TextBox* GBufferTab = TabsGUI->getTab("GBuffer");
    ((Box*)GBufferTab->findChildByID("Positions"))->setTexture(pMainRenderer->getGBuffer()->getPositionMap());
    ((Box*)GBufferTab->findChildByID("Normals"))->setTexture(pMainRenderer->getGBuffer()->getNormalMap());
    ((Box*)GBufferTab->findChildByID("Diffuse"))->setTexture(pMainRenderer->getGBuffer()->getDiffuseMap());
    ((Box*)GBufferTab->findChildByID("IndividualColor"))->setTexture(pMainRenderer->getGBuffer()->getIndividualColorMap());
    ((Box*)GBufferTab->findChildByID("SSAO"))->setTexture(pMainRenderer->getSSAO()->getResultTexture());
    ((Box*)GBufferTab->findChildByID("Objectdata"))->setTexture(pMainRenderer->getGBuffer()->getObjectDataMap());
    ((Box*)GBufferTab->findChildByID("ShadowMaps"))->setTexture(pMainRenderer->getShadowMapping()->getResultTexture(0));*/

    /*float* fpsValue = new float(0);
    Graph* pFPSGraph = new Graph("FPS Graph", ivec2(0, 0), ivec2(600, 200), fpsValue);
    pFPSGraph->setColor(vec4(0.2, 0.6, 0.86, 1));
    pFPSGraph->setPrecision(0);
    GumEngine::GUIS->addGUI(pFPSGraph);*/

    /*TextBox* ExamplesTab = TabsGUI->getTab("Examples");
    for(auto example : mWorlds)
    {
        ((Dropdown*)ExamplesTab->findChildByID("examplesdropdown"))->addEntry(example.first, [example, this](std::string title) {
            pMainRenderer->setWorld(getExampleWorld(example.first));
        });
    }*/

    pMainRenderer->setWorld(getExampleWorld("BasicCube"));
    pMainRenderer->setExposure(1.0f);
    

    while(pMainWindow->isOpen())
    {
        Gum::Display::pollEvents();
        pMainWindow->setClearColor(Gum::GUI::getTheme()->backgroundColor);
        pMainWindow->clear(GL_COLOR_BUFFER_BIT);
        
        //GumEngine::update(GumEngine::ALL);
        pMainCamera->update();
        pMainRenderer->render();

        pGUI->render();
        pGUI->update();

        //System.out.println("Mouse is: " + (Mouse.isBusy() ? "Busy" : "Available"));
        /*if(pRenderCanvas->isMouseInside() && pMainWindow->getMouse()->hasRightClick())
        {
            pMainWindow->getMouse()->hide(true);
            pMainWindow->getMouse()->trap(true);
            Camera::ActiveCamera->setMode(Camera::THIRDPERSON);
        }
        else 
        {
            pMainWindow->getMouse()->hide(false);
            pMainWindow->getMouse()->trap(false);
            Camera::ActiveCamera->setMode(Camera::STATIC);
        }*/
        //GumEngine::DefaultRenderer->getWorld()->getCamera()->setPosition(vec3(std::cos(fTime) * 15, 4, std::sin(fTime) * 15));

        pMainWindow->finishRender();
        pMainWindow->getMouse()->reset();
        pMainWindow->getKeyboard()->reset();

        //pMainWindow.update();
        FPS::update();
    }
}

Examples::~Examples()
{
    Gum::_delete(pMainWindow);
    Gum::_delete(pGUI);
    Gum::_delete(pMainCamera);
    Gum::_delete(pMainRenderer);

    for(auto world : mWorlds)
    {
        Gum::Output::print("Deleting " + world.first);
        Gum::_delete(world.second);
    }
}


World* Examples::getExampleWorld(std::string name)
{
    if(!Tools::mapHasKey(mWorlds, name))
    {
        //if(name == "AnimatesModels")            { mWorlds[name] = createAnimatedModelExample(); }
        //if(name == "Billboards")                { mWorlds[name] = createBillboardsExample(); }
        if(name == "GBuffer")                   { mWorlds[name] = createGBufferExample(); }
        if(name == "BasicCube")                 { mWorlds[name] = createBasicCubeExample(); }
        //if(name == "GUIs")                      { mWorlds[name] = createGUIsExample(); }
        //if(name == "GumMaths")                  { mWorlds[name] = createGumMathsExample(); }
        //if(name == "IrradiancePBR")             { mWorlds[name] = createIrradiancePBRExample(); }
        //if(name == "MazeLearning")              { mWorlds[name] = createMazeLearningExample(); }
        //if(name == "Ocean")                     { mWorlds[name] = createOceanExample(); }
        //if(name == "Particles")                 { mWorlds[name] = createParticlesExample(); }
        //if(name == "PhysicallyBasedRendering")  { mWorlds[name] = createPhysicallyBasedRenderingExample(); }
        //if(name == "Physics")                   { mWorlds[name] = createPhysicsExample(); }
        //if(name == "PostProcessing")            { mWorlds[name] = createPostProcessingExample(); }
        //if(name == "ShadowMapping")             { mWorlds[name] = createShadowMappingExample(); }
        //if(name == "SphereGenerator")           { mWorlds[name] = createSphereGeneratorExample(); }
    }
    return mWorlds[name];
}


int main(int argc, char** argv)
{
    Examples examples;
    return 0;
}