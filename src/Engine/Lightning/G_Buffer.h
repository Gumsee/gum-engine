#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/Texture.h>
#include <Graphics/ShaderProgram.h>
#include <chrono>

class G_Buffer
{
private:
    Framebuffer* gBuffer;
	long long microseconds;
	int depthBuffer;
	std::chrono::high_resolution_clock::time_point start;

    ShaderProgram *pShader;
    void initShader();

public:
    G_Buffer(const ivec2& canvassize);
    ~G_Buffer();

    void bind();
    void unbind();

	//Getter
	int getDepthBuffer();
    ShaderProgram *getShader();
    Texture *getPositionMap();
    Texture *getNormalMap();
    Texture *getDiffuseMap();
	Texture *getObjectDataMap();
	Texture *getDepthMap();
	long long getExecutionTime();
	Framebuffer* getFramebuffer();
};