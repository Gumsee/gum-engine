#pragma once
#include <Graphics/ShaderProgram.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/Texture.h>
#include <Desktop/Window.h>
#include <functional>

class Renderer3D;

class ShadowMapping
{
private:
	static inline ShaderProgram* pShader = nullptr;
    void initShader();

    Framebuffer* pFramebuffer;
    std::vector<float> vShadowCascadeLevels;
    std::vector<mat4> vLightMatrices;

    std::vector<vec4> getFrustumCornersWorldSpace(mat4& proj, const mat4& view);
    mat4 getLightSpaceMatrix(const vec3& lightdir, const float nearPlane, const float farPlane);
    void getLightSpaceMatrices(const vec3& lightdir);

public:
	ShadowMapping();
	~ShadowMapping();

	void render(const vec3& lightdir, const std::function<void()>& renderfunc);


	//Getter
	Texture* getResultTexture();
	static ShaderProgram* getShader();
	std::vector<mat4>& getMatrices();
	std::vector<float>& getCascadeLevels();
};

