#pragma once
#include <GUI/Primitives/Box.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/Texture.h>
#include <Graphics/ShaderProgram.h>
#include <chrono>

class G_Buffer
{
private:
    Box* pRenderCanvas;
    Framebuffer* gBuffer;
	long long microseconds;
	int depthBuffer;
	std::chrono::high_resolution_clock::time_point start;

    ShaderProgram *pShader;
    void initShader();

public:
    G_Buffer(Box* canvas);
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