#pragma once
#include <map>
#include <string>
#include <Engine/General/World.h>
#include <gum-engine.h>
#include <gum-desktop.h>
#include <gum-gui.h>

class Examples
{
private:
    std::map<std::string, World*> mWorlds;
    Gum::Window* pMainWindow;
    Gum::GUI* pGUI;
    Box* pRenderCanvas;
    Camera* pMainCamera;
    Renderer3D* pMainRenderer;

    World* getExampleWorld(std::string name);

public:
    Examples();
    ~Examples();
};