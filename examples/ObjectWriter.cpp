#include "System/Output.h"
#include <Essentials/ArgumentParser.h>
#include <Essentials/Tools.h>
#include <Graphics/Object3D.h>

int main(int argc, char** argv)
{
    Gum::Graphics::RUN_HEADLESS = true;

    Gum::ArgumentParser* pArgumentParser = new Gum::ArgumentParser();
    std::string name = "Object3D";
    Gum::Filesystem::File inputfile, outputfile;

    pArgumentParser->usageSetup("Usage: ObjectWriter [inputfile] [outputfile]", "", "--help", "-h", 20);

    pArgumentParser->addArgument(Gum::Argument("--name=", "-m", "Object name",       [&name](std::string param) { name = param; }));

    if(!pArgumentParser->passArguments(argc, argv, [&inputfile, &outputfile](std::string arg) { if(inputfile.isEmpty()) inputfile = arg; else outputfile = arg; }))
    {
        delete pArgumentParser;
        return 1;
    }
    Gum::_delete(pArgumentParser);
    
    if(inputfile.isEmpty())
    {
        std::cerr << "Please specify an input file" << std::endl;
        return 1;
    }
    
    if(outputfile.isEmpty())
    {
        std::cerr << "Please specify an input file" << std::endl;
        return 1;
    }

    Object3D* obj = new Object3D(inputfile, name);
    obj->saveToFile(outputfile);

    return 0;
}