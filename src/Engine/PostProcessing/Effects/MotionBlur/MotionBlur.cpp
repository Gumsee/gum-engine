#include "MotionBlur.h"
#include "MotionBlurShader.h"
#include "../../PostProcessing.h"
#include "../../../Rendering/Camera.h"

MotionBlur::MotionBlur(Canvas* canvas)
{
	init(canvas);

  this->pShader = ShaderProgram::requestShaderProgram("MotionBlurShader", true);
  this->pShader->addShader(Gum::PostProcessing::VertexShader);
  this->pShader->addShader(Shader::requestShader("MotionBlurShader", MotionBlurFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
  this->pShader->build();

  this->pShader->addTexture("texture0", 0);
}


MotionBlur::~MotionBlur() { }

Texture* MotionBlur::render(Texture* texture)
{
  vec3 diff = Camera::getActiveCamera()->getViewDirection() - vOldCameraDirection;

	pFramebuffer->bind();
	pFramebuffer->clear(Framebuffer::ClearFlags::COLOR);
	pShader->use();
  pShader->loadUniform("velocity", );
	texture->bind(0);
	pRenderCanvas->render();
	texture->unbind(0);
	pShader->unuse();
	pFramebuffer->unbind();

  vOldCameraDirection = Camera::getActiveCamera()->getViewDirection();
    
  return pFramebuffer->getTextureAttachment(0);
}