#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/ShaderProgram.h>
#include <GUI/Primitives/Box.h>

#include "../Lightning/G_Buffer.h"

class Renderer;

class SSAO
{
private:
	Texture2Df *pNoiseTexture;
	ShaderProgram *pShader;
	ShaderProgram *pBlurShader;

	Renderer* pRenderer;

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
	SSAO(Box *gui, G_Buffer *gbuffer, Renderer* renderer);
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
	Texture2Df* getResultTexture();
	Texture2Df* getNoiseTexture();
};

