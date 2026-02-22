#include <cstdint>
#include <gum-engine.h>
#include <gum-graphics.h>
#include <gum-system.h>
#include <gum-essentials.h>
#include <gum-gui.h>
#include <gum-desktop.h>
#include <GUI/Basics/Basics.h>
#include <string>
#include "Desktop/IO/Mouse.h"
#include <Codecs/Zip.h>
#include "Engine/3D/Lightning/ShadowMapping/ShadowMapping.h"
#include "Engine/Material/MaterialManager.h"
#include "Engine/PostProcessing/Effects/Effects.h"
#include "Engine/Texture/TextureManager.h"
#include "Essentials/Time.h"
#include "Essentials/Tools.h"
#include "Essentials/Unicode.h"
#include "Essentials/Noise.h"
#include "GUI/Basics/Dropdown.h"
#include "GUI/Primitives/RenderGUI.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Texture2D.h"
#include "Maths/Maths.h"
#include "Noises.h"
#include "System/File.h"
#include "System/Output.h"
#include "Worlds/TestScene.h"
#include "Worlds/Vehicle.h"
#include "Worlds/Worlds.h"
#include "Worlds/ShadowMapping.h"
#include "Worlds/PostProcessingEffects.h"
#include <Essentials/Serialization.h>
#include "Examples.h"

/******

TODO
    Shader Editor (Nodes)
    Selecting Objects (Fix individual colors)
    Global ObjectManager
    Render ObjectIDs

 ******/

std::map<std::string, World*> mWorlds;
Gum::Window* pMainWindow;
Gum::GUI* pGUI;
Box* pRenderCanvas;
Camera3D* pMainCamera;
Renderer3D* pMainRenderer;


World* getExampleWorld(std::string name)
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
        if(name == "PhysicallyBasedRendering")  { mWorlds[name] = World::readFromFile(Examples::assetPath + "worlds/physicallybased.wld"); }
        if(name == "Physics")                   { mWorlds[name] = createPhysicsExample(); }
        if(name == "PostProcessing")            { mWorlds[name] = createPostProcessingExample(); }
        if(name == "Vehicle")                   { mWorlds[name] = createVehicleExample(); }
        if(name == "ShadowMapping")             { mWorlds[name] = createShadowMappingExample(); }
        if(name == "TestScene")                 { mWorlds[name] = createTestSceneExample(); }
        //if(name == "SphereGenerator")           { mWorlds[name] = createSphereGeneratorExample(); }
    }
    return mWorlds[name];
}

Gum::Filesystem::File Examples::assetPath = Gum::Filesystem::File(std::string(EXAMPLES_PATH) + "/assets/", Gum::Filesystem::DIRECTORY);

int main(int argc, char** argv)
{
    Gum::Display::init();
    pMainWindow = new Gum::Window("Gumball Examples", ivec2(75, 75), GUM_WINDOW_SIZE_IN_PERCENT | GUM_WINDOW_RESIZABLE);
    pMainWindow->setVerticalSync(false);
	pMainWindow->getMouse()->trap(false);
	pMainWindow->getMouse()->hide(false);
    Gum::Graphics::addFramebufferToWindow(pMainWindow);

    pGUI = new Gum::GUI(pMainWindow);
    Framebuffer::CurrentlyBoundFramebuffer->setClearColor(Gum::GUI::getTheme()->backgroundColor);

    ObjectManager::MODEL_ASSETS_PATH = Examples::assetPath + Gum::File("/objects/", Gum::Filesystem::DIRECTORY);;
    Gum::TextureManager::TEXTURE_ASSETS_PATH = Examples::assetPath + Gum::File("/textures/", Gum::Filesystem::DIRECTORY);;
    Gum::MaterialManager::MATERIAL_ASSETS_PATH = Examples::assetPath + Gum::File("/materials/", Gum::Filesystem::DIRECTORY);
    Gum::Engine::init();

    XMLLoader pXMLGUILoader(Examples::assetPath.toString() + "/guis/example_interface.xml");
    pGUI->addGUI(pXMLGUILoader.getRootGUI());
    pRenderCanvas = (Box*)pXMLGUILoader.getRootGUI()->findChildByID("renderview");

    //Gum::Output::print(pXMLGUILoader.getRootGUI()->getHierarchy());

    pMainCamera = new Camera3D(pRenderCanvas->getSize(), new World3D());
    pMainCamera->setPosition(vec3(0,0,0));
    pMainCamera->setMode(Camera3D::Modes::STATIC);
    pMainCamera->setOffset(60.0f);
    pMainCamera->setZoomFactor(3.0f);
    pMainCamera->makeActive();
    //pMainCamera->setRotationalSpeed(0.2f);

    pMainRenderer = new Renderer3D(pRenderCanvas);
    pMainRenderer->setWorld((World3D*)getExampleWorld("BasicCube"));
    pMainRenderer->setExposure(1.0f);

    pMainWindow->onResized([](ivec2 size) {
        pGUI->setSize(size);
        ((Camera3D*)Camera::getActiveCamera())->updateProjection(pRenderCanvas->getSize());
        pMainRenderer->updateFramebufferSize();
    });

    Settings::setSetting(Settings::SHOWDEBUGINFO, true);


    Tabs* TabsGUI = (Tabs*)pXMLGUILoader.getRootGUI()->findChildByID("sidemenutabs");
    RenderGUI* GBufferTab = TabsGUI->getTabContent("Renderdata");
    ((Box*)GBufferTab->findChildByID("Positions"))->setTexture(pMainRenderer->getGBuffer()->getPositionMap());
    ((Box*)GBufferTab->findChildByID("Normals"))->setTexture(pMainRenderer->getGBuffer()->getNormalMap());
    ((Box*)GBufferTab->findChildByID("Diffuse"))->setTexture(pMainRenderer->getGBuffer()->getDiffuseMap());
    ((Box*)GBufferTab->findChildByID("IndividualColor"))->setTexture(pMainRenderer->getIDRenderer()->getResultTexture());
    ((Box*)GBufferTab->findChildByID("SSAO"))->setTexture(pMainRenderer->getSSAO()->getResultTexture());
    ((Box*)GBufferTab->findChildByID("Objectdata"))->setTexture(pMainRenderer->getGBuffer()->getObjectDataMap());
    ((Box*)GBufferTab->findChildByID("Depth"))->setTexture(pMainRenderer->getGBuffer()->getDepthMap());
    //((Box*)GBufferTab->findChildByID("ShadowMaps"))->setTexture(pMainRenderer->getShadowMapping()->getResultTexture());

    /*float* fpsValue = new float(0);
    Graph* pFPSGraph = new Graph("FPS Graph", ivec2(0, 0), ivec2(600, 200), fpsValue);
    pFPSGraph->setColor(vec4(0.2, 0.6, 0.86, 1));
    pFPSGraph->setPrecision(0);
    GumEngine::GUIS->addGUI(pFPSGraph);*/

    RenderGUI* examplesTab = TabsGUI->getTabContent("Examples");
    Dropdown* examplesDropdown = ((Dropdown*)examplesTab->findChildByID("examplesdropdown"));
    examplesDropdown->onSelection([](Gum::Unicode title) {
        pMainRenderer->setWorld((World3D*)getExampleWorld(title.toString()));
    });

    for(std::string example : { "TestScene", "AnimatesModels", "Billboards", "GBuffer", "BasicCube", "IrradiancePBR", "MazeLearning", "Ocean", "Particles", "PhysicallyBasedRendering", "Physics", "PostProcessing", "ShadowMapping", "SphereGenerator", "Vehicle" })
        examplesDropdown->addEntry(example, false);


    Dropdown* postprocessingDropdown = ((Dropdown*)examplesTab->findChildByID("postprocessingdropdown"));
    postprocessingDropdown->onSelection([](Gum::Unicode title) {

    });

    for(std::string effect : { "GaussianBlur" })
        postprocessingDropdown->addEntry(effect, false);

    pMainWindow->getMouse()->onRelease([](int button, int mods) {
        if(button & GUM_MOUSE_BUTTON_RIGHT)
        {
            pMainWindow->getMouse()->trap(false);
            ((Camera3D*)Camera::getActiveCamera())->setMode(Camera3D::STATIC);
        }
    });

    RenderGUI* noisepatternsTab = TabsGUI->getTabContent("2D Noisepatterns");
    buildNoiseTextureSection(noisepatternsTab);

    //pMainRenderer->addPostProcessingEffect(new GaussianBlur(pRenderCanvas, 10));
    //pMainRenderer->addPostProcessingEffect(new Brightfilter(pRenderCanvas));
    //pMainRenderer->addPostProcessingEffect(new BloomEffect(pRenderCanvas));

    //getExampleWorld("PhysicallyBasedRendering")->saveToFile(assetPath + Gum::Filesystem::File("/worlds/physicallybased.wld"));

    int oldtime = 0;
    while(pMainWindow->isOpen())
    {
        Gum::Display::pollEvents();
        Framebuffer::CurrentlyBoundFramebuffer->clear(Framebuffer::ClearFlags::COLOR);
        
        pMainCamera->update();
        pMainRenderer->update();
        pMainRenderer->render();
        pMainRenderer->renderIDs();

        pMainWindow->bind();
        pGUI->render();
        pGUI->update();

        //vec4 campos = Camera::getActiveCamera()->getViewMatrix() * vec4(0, 0, 0, 1);
        if((int)Time::getTime() != oldtime)
        {
            Gum::Output::print(Time::getFPS());
            oldtime = (int)Time::getTime();
        }

        if(pRenderCanvas->hasClickedInside(GUM_MOUSE_BUTTON_RIGHT))
        {
            pMainWindow->getMouse()->trap(true);
            ((Camera3D*)Camera::getActiveCamera())->setMode(Camera3D::FREECAM);
        }

        pMainWindow->finishRender();
        pMainWindow->getMouse()->reset();
        pMainWindow->getKeyboard()->reset();

        //pMainWindow.update();
        Texture::loadTextures();
        Time::update();
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