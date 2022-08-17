#include "MaterialManager.h"
#include <Essentials/MemoryManagement.h>
#include <map>

namespace Gum {
namespace MaterialManager
{
	std::map<std::string, Material*> Materials;
	Material *pDefaultMaterial;

    void init()
    {
        pDefaultMaterial = new Material();
        pDefaultMaterial->setName("default");
        addMaterial(pDefaultMaterial, "default");
    }


    void cleanup()
    {
        Gum::_delete(pDefaultMaterial);
        Materials.clear();
    }
    

    void addMaterial(Material *Mat, std::string Identifier) { Materials[Identifier] = Mat; }
    void deleteMaterial(std::string MatName) { Materials.erase(MatName); }
    void deleteMaterial(Material* matPtr)
    {
        for(std::map<std::string, Material*>::iterator it = Materials.begin(); it != Materials.end(); it++)
        {
            if(it->second == matPtr)
            {
                Materials.erase(it);
                break;
            }
        }
    }
    
    int getCount() { return Materials.size(); }
    Material *getDefaultMaterial() { return pDefaultMaterial; }
    Material *getMaterial(std::string matname) { return Materials[matname]; }
}}