#pragma once
#include <map>
#include <string>
#include <Engine/General/World.h>

class Examples
{
private:
    std::map<std::string, World*> mWorlds;

    World* getExampleWorld(std::string name);

public:
    Examples();
    ~Examples();
};