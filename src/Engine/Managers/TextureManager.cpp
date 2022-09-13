#include "TextureManager.h"
#include <Essentials/Tools.h>
#include <System/MemoryManagement.h>

namespace Gum {
namespace TextureManager
{
	std::string TEXTURE_ASSETS_PATH = "";
	std::map<std::string, Texture*> mTextures;
	std::vector<Texture*> vTexturesToLoad;

	/*
	* This method loads a texture into memory 
	*/
	Texture *loadTexture(std::string name, bool waitForLoading)
	{
		std::string extension = Tools::toUpperCase(name.substr(name.length() - 3, 3));
		if(extension == "HDR")
		{
			TextureHDR *tex = new TextureHDR(name);
			tex->load(TEXTURE_ASSETS_PATH + name, waitForLoading);
			mTextures[name] = tex;
			return tex;
		}
		else
		{
			Texture2D *tex = new Texture2D(name);
			tex->load(TEXTURE_ASSETS_PATH + name, waitForLoading);
			mTextures[name] = tex;
			return tex;
		}
	}

	void update()
	{
		for(size_t i = 0; i < vTexturesToLoad.size(); i++)
		{
			vTexturesToLoad[i]->updateImage();
			vTexturesToLoad.erase(vTexturesToLoad.begin() + i);
		}
	}

	void cleanup()
	{
		for(auto tex : mTextures)
			Gum::_delete(tex.second);
		mTextures.clear();
	}

	void deleteTexture(std::string TexName) 				{ Gum::_delete(mTextures[TexName]); mTextures.erase(TexName); }
	void addTexture(Texture *tex, std::string Identifier) 	{ mTextures[Identifier] = tex; }
	void addTextureToLoad(Texture* tex)                     { vTexturesToLoad.push_back(tex); }

	//
	// Getter
	//
	int getNumTexture() 								    { return mTextures.size(); }

	Texture *getTexture(std::string texname, bool waitForLoading)
	{
		if (mTextures.find(texname) == mTextures.end()) 
			mTextures[texname] = loadTexture(texname, waitForLoading);

		return mTextures[texname];
	}
}}