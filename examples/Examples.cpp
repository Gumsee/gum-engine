#include <gum-engine.h>
#include <gum-opengl.h>
#include <gum-system.h>
#include <gum-essentials.h>
#include <gum-gui.h>
#include <gum-desktop.h>
#include <GUI/Basics/Basics.h>
#include <string>
#include "Desktop/IO/Mouse.h"
#include "Engine/Texture/TextureManager.h"
#include "Essentials/Unicode.h"
#include "GUI/Basics/Dropdown.h"
#include "GUI/Primitives/RenderGUI.h"
#include "System/Output.h"
#include "Worlds/Worlds.h"

/******

TODO
    Shader Editor (Nodes)
    Selecting Objects (Fix individual colors)
    Global ObjectManager
    Render ObjectIDs

 ******/

std::map<std::string, World3D*> mWorlds;
Gum::Window* pMainWindow;
Gum::GUI* pGUI;
Box* pRenderCanvas;
Camera3D* pMainCamera;
Renderer3D* pMainRenderer;


World3D* getExampleWorld(std::string name)
{
    if(!Tools::mapHasKeyNotNull(mWorlds, name))
    {
        if(name == "AnimatesModels")            { mWorlds[name] = createAnimatedModelExample(); }
        if(name == "Billboards")                { mWorlds[name] = createBillboardsExample(); }
        if(name == "GBuffer")                   { mWorlds[name] = createGBufferExample(); }
        if(name == "BasicCube")                 { mWorlds[name] = createBasicCubeExample(); }
        //if(name == "IrradiancePBR")             { mWorlds[name] = createIrradiancePBRExample(); }
        //if(name == "MazeLearning")              { mWorlds[name] = createMazeLearningExample(); }
        //if(name == "Ocean")                     { mWorlds[name] = createOceanExample(); }
        if(name == "Particles")                 { mWorlds[name] = createParticlesExample(); }
        if(name == "PhysicallyBasedRendering")  { mWorlds[name] = createPhysicallyBasedRenderingExample(); }
        if(name == "Physics")                   { mWorlds[name] = createPhysicsExample(); }
        //if(name == "PostProcessing")            { mWorlds[name] = createPostProcessingExample(); }
        //if(name == "ShadowMapping")             { mWorlds[name] = createShadowMappingExample(); }
        //if(name == "SphereGenerator")           { mWorlds[name] = createSphereGeneratorExample(); }
    }
    return mWorlds[name];
}


int main(int argc, char** argv)
{
    std::string example_path = std::string(EXAMPLES_PATH) + "/assets/";

    Gum::Display::init();
    pMainWindow = new Gum::Window("GUI Example", ivec2(75, 75), GUM_WINDOW_SIZE_IN_PERCENT | GUM_WINDOW_RESIZABLE);
    pMainWindow->setClearColor(color(61, 99, 191, 255));
    pMainWindow->setVerticalSync(true);
	pMainWindow->getMouse()->trap(false);
	pMainWindow->getMouse()->hide(false);
    Gum::OpenGL::addFramebufferToWindow(pMainWindow);

    pGUI = new Gum::GUI(pMainWindow);

    ObjectManager::MODEL_ASSETS_PATH = example_path + "objects/";
    Gum::TextureManager::TEXTURE_ASSETS_PATH = example_path + "textures/";
    Gum::Engine::init();

    XMLLoader pXMLGUILoader(example_path + "/guis/example_interface.xml");
    pGUI->addGUI(pXMLGUILoader.getRootGUI());
    pRenderCanvas = (Box*)pXMLGUILoader.getRootGUI()->findChildByID("renderview");

    pMainRenderer = new Renderer3D(pRenderCanvas);
    pMainRenderer->setWorld(getExampleWorld("BasicCube"));
    pMainRenderer->setExposure(1.0f);

    pMainWindow->onResized([](ivec2 size) {
        pGUI->setSize(size);
        pMainRenderer->updateFramebufferSize();
    });

    Settings::setSetting(Settings::SHOWDEBUGINFO, true);

    pMainCamera = new Camera3D(pMainWindow->getSize(), new World3D());
    pMainCamera->setPosition(vec3(0,0,0));
    pMainCamera->setMode(Camera3D::Modes::STATIC);
    pMainCamera->setOffset(60.0f);
    pMainCamera->setZoomFactor(3.0f);
    //pMainCamera->setRotationalSpeed(0.2f);

    /*Box* debugBox = new Box(ivec2(100, 100), ivec2(300, 300));
    debugBox->setTexture(Gum::TextureManager::getTexture("boii.jpg"));
    pGUI->addGUI(debugBox);*/


    /*FileExplorer* pFileExplorer = new FileExplorer(ivec2(0,0), ivec2(100, 100), "./");
    pFileExplorer->setSizeInPercent(true, true);
    pXMLGUILoader->getGUI()->findChildByID("filesmenu")->addGUI(pFileExplorer);*/


    Tabs* TabsGUI = (Tabs*)pXMLGUILoader.getRootGUI()->findChildByID("sidemenutabs");
    RenderGUI* GBufferTab = TabsGUI->getTabContent("GBuffer");
    ((Box*)GBufferTab->findChildByID("Positions"))->setTexture(pMainRenderer->getGBuffer()->getPositionMap());
    ((Box*)GBufferTab->findChildByID("Normals"))->setTexture(pMainRenderer->getGBuffer()->getNormalMap());
    ((Box*)GBufferTab->findChildByID("Diffuse"))->setTexture(pMainRenderer->getGBuffer()->getDiffuseMap());
    ((Box*)GBufferTab->findChildByID("IndividualColor"))->setTexture(pMainRenderer->getIDRenderer()->getResultTexture());
    ((Box*)GBufferTab->findChildByID("SSAO"))->setTexture(pMainRenderer->getSSAO()->getResultTexture());
    ((Box*)GBufferTab->findChildByID("Objectdata"))->setTexture(pMainRenderer->getGBuffer()->getObjectDataMap());
    ((Box*)GBufferTab->findChildByID("Depth"))->setTexture(pMainRenderer->getGBuffer()->getDepthMap());
    //((Box*)GBufferTab->findChildByID("ShadowMaps"))->setTexture(pMainRenderer->getShadowMapping()->getResultTexture(0));

    /*float* fpsValue = new float(0);
    Graph* pFPSGraph = new Graph("FPS Graph", ivec2(0, 0), ivec2(600, 200), fpsValue);
    pFPSGraph->setColor(vec4(0.2, 0.6, 0.86, 1));
    pFPSGraph->setPrecision(0);
    GumEngine::GUIS->addGUI(pFPSGraph);*/

    RenderGUI* examplesTab = TabsGUI->getTabContent("Examples");
    Dropdown* examplesDropdown = ((Dropdown*)examplesTab->findChildByID("examplesdropdown"));
    examplesDropdown->onSelection([](Gum::Unicode title) {
        pMainRenderer->setWorld(getExampleWorld(title.toString()));
    });
    for(std::string example : { "AnimatesModels", "Billboards", "GBuffer", "BasicCube", "IrradiancePBR", "MazeLearning", "Ocean", "Particles", "PhysicallyBasedRendering", "Physics", "PostProcessing", "ShadowMapping", "SphereGenerator" })
        examplesDropdown->addEntry(example, false);
    pMainWindow->getMouse()->onRelease([](int button, int mods) {
        if(button & GUM_MOUSE_BUTTON_RIGHT)
        {
            pMainWindow->getMouse()->trap(false);
            ((Camera3D*)Camera::getActiveCamera())->setMode(Camera3D::STATIC);
        }
    });

    while(pMainWindow->isOpen())
    {
        Gum::Display::pollEvents();
        pMainWindow->setClearColor(Gum::GUI::getTheme()->backgroundColor);
        pMainWindow->clear(GL_COLOR_BUFFER_BIT);
        
        //GumEngine::update(GumEngine::ALL);
        pMainCamera->update();
        pMainRenderer->update();
        pMainRenderer->render();

        pGUI->render();
        pGUI->update();

        if(pRenderCanvas->hasClickedInside(GUM_MOUSE_BUTTON_RIGHT))
        {
            pMainWindow->getMouse()->trap(true);
            ((Camera3D*)Camera::getActiveCamera())->setMode(Camera3D::THIRDPERSON);
        }

        pMainWindow->finishRender();
        pMainWindow->getMouse()->reset();
        pMainWindow->getKeyboard()->reset();

        //pMainWindow.update();
        Texture::loadTextures();
        FPS::update();
    }


    Gum::_delete(pMainWindow);
    Gum::_delete(pGUI);
    Gum::_delete(pMainCamera);
    Gum::_delete(pMainRenderer);

    for(auto world : mWorlds)
    {
        Gum::Output::print("Deleting " + world.first);
        Gum::_delete(world.second);
    }

    Gum::Engine::cleanup();
    return 0;
}