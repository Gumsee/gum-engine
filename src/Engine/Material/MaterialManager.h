#pragma once
#include <Graphics/Material.h>

namespace Gum {
namespace MaterialManager
{
	extern Gum::File MATERIAL_ASSETS_PATH;

	extern void init();

	/**
	 * Cleans Materials
	 */
	extern void cleanup();

	/**
	 * Adds a Material that can be Called later in the Program with spezific Identifier
	 */
	extern void addMaterial(Material *Mat, std::string Identifier);

	/**
	 * Deletes a spezifified Material by Name
	 */
	extern void deleteMaterial(std::string MatName);

	/**
	 * Deletes a spezifified Material by Pointer
	 */
	extern void deleteMaterial(Material* matPtr);

	/**
	 * Returns default material
	 */
	extern Material *getDefaultMaterial();

	/**
	 * Returns a Material by name
	 */
	extern Material *getMaterial(std::string matname);

	/**
	 * Returns the Count of Materials in the Manager
	 */
	extern int getCount();
}};