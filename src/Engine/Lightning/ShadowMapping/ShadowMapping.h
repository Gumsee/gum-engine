#pragma once
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>
#include <OpenGL/Texture.h>
#include <OS/Window.h>

#include "../../General/Camera.h"
#include "ShadowBox.h"

class Renderer3D;

class ShadowMapping
{
private:
	std::vector<mat4> GetMatrixProduct;
	std::vector<Framebuffer*> vFramebuffers;
	std::vector<Texture2D*> texture;
	std::vector<float> textureOffsets;
	mat4 projectionViewMatrix, lightViewMatrix, projectionMatrix, m4BiasMatrix;
	ShadowBox *box;

	int ShadowSize;
	int ShadowMapAmount;
	ShaderProgram *pShader;
    void initShader();
    
public:
	ShadowMapping(Renderer3D* renderer);
	~ShadowMapping();

	void prepare(vec3 LightDirection, int index);
	void finish();
	void addShadowMap(std::string name, float offset);
	void updateLightViewMatrix(vec3 direction, vec3 center);
	void updateOrthoProjectionMatrix(float width, float height, float length);

    Texture2D* createDepthTextureAttachment(Framebuffer* framebuffer);
	

	//Getter
	Texture2D* getResultTexture(int Index);
	ShaderProgram* getShader();
	mat4* getMatrix();
	int* getShadowMapSize();
};

