#pragma once
#include <OpenGL/Texture2D.h>
#include <OpenGL/TextureHDR.h>

#include <map>
#include <vector>

namespace Gum {
namespace TextureManager
{
	extern std::string TEXTURE_ASSETS_PATH;

	/*
	* Returns a Texture by name
	*/
	extern Texture *getTexture(std::string texname, bool waitForLoading = false);

	/*
	* Deletes a spezifified Texture
	*/
	extern void deleteTexture(std::string TexName);

	/*
	* Adds a Texture that can be Called later in the Program with spezific Identifier
	*
	* Warning: Identifier shouldn't be used twice
	*/
	extern void addTexture(Texture *tex, std::string Identifier);


	/*
	* This method is automatically called by the Engine (if initialized)
	*
	* Checks every frame if textures have been loaded and adds them
	*
	* Note: this gets called by Engine::update(Engine::UPDATEABLE)
	*/
	extern void update();


    extern void addTextureToLoad(Texture* tex);

	/*
	* This function gets called the Engine never call it yourself
	*/
	extern void cleanup();


	//Getter	
	/*
	* Returns the whole map of Textures and Identifieres
	*/
	extern std::map<std::string, Texture*> *getTextureMap();
	extern int getNumTexture();
}}