#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/ShaderProgram.h>
#include <GUI/Primitives/Box.h>

#include "../Lightning/G_Buffer.h"
#include "../General/Renderer3D.h"

class SSAO
{
private:
	Texture2D *pNoiseTexture;
	ShaderProgram *pShader;
	ShaderProgram *pBlurShader;

	Renderer3D* pRenderer;

	Box *rect;
	G_Buffer *gbuffer;

	unsigned int kernelSize;
	unsigned int NoiseSize;
	float power;
	float radius;
	bool generatedNoise;
	bool generatedKernel;

	vec2 noiseScale;

    Framebuffer *pSSAOFramebuffer;
    Framebuffer *pSSAOBlurFramebuffer;

	std::vector<vec3> ssaoKernel;
	std::vector<float> ssaoNoise;
    long lSeed = 10L;

	long long microseconds;
	void generateKernel();
    void initShader();

public:
	SSAO(Box *gui, G_Buffer *gbuffer, Renderer3D* renderer);
	~SSAO();

	void render();

	void setPower(const float& power);
	void setKernelSize(const unsigned int& kernelsize);
	void setNoiseSize(const unsigned int& noisesize);
	void setRadius(const float& radius);

	float getPower() const;
	int getKernelSize() const;
	int getNoiseSize() const;
	float getRadius() const;
	long long getExecutionTime() const;
	Texture2D* getResultTexture();
	Texture2D* getNoiseTexture();
};

