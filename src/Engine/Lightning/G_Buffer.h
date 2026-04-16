#pragma once
#include <Graphics/Framebuffer.h>
#include <Graphics/Texture.h>
#include <Graphics/ShaderProgram.h>
#include <Essentials/Clock.h>

class G_Buffer
{
private:
  Framebuffer* gBuffer;
	Clock<> clock;

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