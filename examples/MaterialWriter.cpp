#include <Graphics/Texture2D.h>
#include <System/Output.h>
#include <Essentials/ArgumentParser.h>
#include <Essentials/Tools.h>
#include <Graphics/Material.h>


void addTex(Material* mat, const Gum::File& file, int type)
{
    if(!file.isEmpty())
    {
        Gum::Output::print(file.toString());
        Texture2D* tex = new Texture2D(file.getName());
        tex->load(file, true);
        mat->setTexture(tex, type);
    }
}

int main(int argc, char** argv)
{
    Gum::Graphics::RUN_HEADLESS = true;

    Gum::ArgumentParser* pArgumentParser = new Gum::ArgumentParser();
    std::string name = "";
    Gum::Filesystem::File filename;
    Gum::Filesystem::File diffusefile;
    Gum::Filesystem::File normalfile;
    Gum::Filesystem::File roughnessfile;
    Gum::Filesystem::File ambientocclusionfile;
    Gum::Filesystem::File heightfile;
    Gum::Filesystem::File specularfile;
    color col;

    pArgumentParser->usageSetup("Usage: MaterialWriter [outputfile]", "", "--help", "-h", 20);
    pArgumentParser->addArgument(Gum::Argument("--name=",      "-m", "Material name",       [&name](std::string param) { name = param; }));
    pArgumentParser->addArgument(Gum::Argument("--diffuse=",   "-d", "Diffuse Texture",     [&diffusefile](std::string param) { diffusefile = param; }));
    pArgumentParser->addArgument(Gum::Argument("--normal=",    "-n", "Normal Texture",      [&normalfile](std::string param) { normalfile = param; }));
    pArgumentParser->addArgument(Gum::Argument("--roughness=", "-r", "Roughness Texture",   [&roughnessfile](std::string param) { roughnessfile = param; }));
    pArgumentParser->addArgument(Gum::Argument("--ambient=",   "-a", "Ambient Occlusion Texture", [&ambientocclusionfile](std::string param) { ambientocclusionfile = param; }));
    pArgumentParser->addArgument(Gum::Argument("--height=",    "-h", "Heightmap Texture",   [&heightfile](std::string param) { heightfile = param; }));
    pArgumentParser->addArgument(Gum::Argument("--specular=",  "-s", "Specular Texture",    [&specularfile](std::string param) { specularfile = param; }));
    pArgumentParser->addArgument(Gum::Argument("--color=",     "-c", "Color",               [&col](std::string param) { vec4 vec = Tools::StringToVec<float, 4>(param); col = rgba(vec.x, vec.y, vec.z, vec.w); }));
    pArgumentParser->addArgument(Gum::Argument("--quality=",   "-q", "Compression Quality", [](std::string param) { Texture2D::setSerializeQuality(Tools::StringToNum<int>(param)); }));


    if(!pArgumentParser->passArguments(argc, argv, [&filename](std::string arg) { filename = arg; }))
    {
        delete pArgumentParser;
        return 1;
    }
    Gum::_delete(pArgumentParser);
    
    if(filename.isEmpty())
    {
        std::cerr << "Please specify a file" << std::endl;
        return 1;
    }

    Material* material = new Material();
    material->setName(name);
	material->setColor(col);
	//material->setRoughness(roughness);
	//material->setSpecularity(specular);

    addTex(material, diffusefile, GUM_MATERIAL_TEXTURE0);
    addTex(material, normalfile, GUM_MATERIAL_NORMAL_MAP);
    addTex(material, roughnessfile, GUM_MATERIAL_ROUGHNESS_MAP);
    addTex(material, ambientocclusionfile, GUM_MATERIAL_AMBIENT_OCCLUSION_MAP);
    addTex(material, heightfile, GUM_MATERIAL_DISPLACEMENT_MAP);
    addTex(material, specularfile, GUM_MATERIAL_SPECULAR_MAP);

    material->saveToFile(filename);

    //material->setTexture(Gum::TextureManager::getTexture("normaltest.jpg", true), GUM_MATERIAL_NORMAL_MAP);
    return 0;
}