#pragma once
#include <Essentials/Time.h>
#include "../../Texture/EnvironmentMap.h"
#include <Graphics/Texture3D.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/TextureCube.h>
#include <Graphics/Object3D.h>
#include "../../Rendering/Canvas.h"

class SkyBox : public Object3D
{
private:
	bool bRenderSky = true;
	bool bRenderClouds = false;

  Texture3Df* cloudNoise3D;
  Texture2Df* cloudNoise2D;
	EnvironmentMapf* pTexture;
  EnvironmentMapf* pIrradianceMap;
	Framebuffer* pBRDFFramebuffer;
	EnvironmentMapf* pPreFilterMap;

	static inline ShaderProgram *HDRShader = nullptr;
	static inline ShaderProgram *CloudShader = nullptr;
	static inline ShaderProgram *HDRToCubeMapShader = nullptr;
	static inline ShaderProgram *IrradianceMapShader = nullptr;
	static inline ShaderProgram *PreFilteredMapShader = nullptr;
	static inline ShaderProgram *BRDFMapShader = nullptr;

	Canvas *pBRDFCanvas;

	void makeIrradianceMap();
	void makeCubeMap(Texture* texture);
	void makePrefilterMap();
	void makeBRDFMap();
	void updateTexture();

	vec3 *sunDir;

  void initShaders();

public:
	SkyBox(Mesh *mesh, vec3 *SunDirection, std::string name);
	~SkyBox();

	void render();

	/*Bind textures to Cube map
	* 1 GL_TEXTURE_CUBE_MAP_POSITIVE_X = Right
	* 2 GL_TEXTURE_CUBE_MAP_NEGATIVE_X = Left
	* 3 GL_TEXTURE_CUBE_MAP_POSITIVE_Y = Top
	* 4 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y = Bottom
	* 5 GL_TEXTURE_CUBE_MAP_POSITIVE_Z = Back
	* 6 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z = Front
	*/
	void setTexture(Texture *tex);
	TextureCubef *getTexture();
	TextureCubef *getIrradianceMap();
	TextureCubef *getPreFilterMap();
	Texture2D *getBRDFConvMap();
	void renderSky(bool rendersky);
	void renderClouds(bool renderclouds);
};