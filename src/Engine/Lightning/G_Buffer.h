#pragma once
#include <OpenGL/Framebuffer.h>
#include <OpenGL/Texture.h>
#include <OpenGL/ShaderProgram.h>
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
    G_Buffer(ivec2 resolution);
    ~G_Buffer();

    void bind();
    void unbind();

	//Getter
	int getDepthBuffer();
    ShaderProgram *getShader();
    Texture *getPositionMap();
	Texture *getIndividualColorMap();
    Texture *getNormalMap();
    Texture *getDiffuseMap();
	Texture *getObjectDataMap();
	Texture *getDepthMap();
	long long getExecutionTime();
	Framebuffer* getFramebuffer();
};