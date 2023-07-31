#pragma once
#include <Essentials/FPS.h>
#include "../Camera3D.h"
#include <OpenGL/Framebuffer.h>
#include <OpenGL/TextureCube.h>
#include <OpenGL/Object3D.h>
#include <GUI/Primitives/Box.h>

class SkyBox : public Object3D
{
private:
	bool gradiant;
	bool isSpinning = false;

	ivec2 v2Resolution;
	ivec2 v2BRDFResolution;
    ivec2 v2PreFilterResolution;
    TextureCube *pTexture;

	Framebuffer* pFramebuffer;
    Framebuffer* pIrradianceFramebuffer;
	Framebuffer* pBRDFFramebuffer;
	Framebuffer* pPreFilterMap;
	unsigned int captureRBOPreFiltered;

	ShaderProgram *HDRToCubeMapShader;
	ShaderProgram *IrradianceMapShader;
	ShaderProgram *PreFilteredMapShader;
	ShaderProgram *BRDFMapShader;

	Box *pBRDFCanvas;

	void makeIrradianceMap();
	void makeCubeMap(Texture* texture);
	void makePrefilterMap();
	void makeBRDFMap();
	void updateTexture();

	mat4 captureProjection;
    std::vector<mat4> captureViews;
	vec3 *sunDir;

    void initShaders();

public:
	SkyBox(Mesh *mesh, vec3 *SunDirection, std::string name);
	~SkyBox();

	void render();
	void update();

	/*Bind textures to Cube map
	* 1 GL_TEXTURE_CUBE_MAP_POSITIVE_X = Right
	* 2 GL_TEXTURE_CUBE_MAP_NEGATIVE_X = Left
	* 3 GL_TEXTURE_CUBE_MAP_POSITIVE_Y = Top
	* 4 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y = Bottom
	* 5 GL_TEXTURE_CUBE_MAP_POSITIVE_Z = Back
	* 6 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z = Front
	*/
	void setTexture(Texture *tex);
	TextureCube *getTexture();
	TextureCube *getIrradianceMap();
	TextureCube *getPreFilterMap();
	Texture2D *getBRDFConvMap();
	void useGradiant(bool gradiant);
	void spin(bool spin);
};