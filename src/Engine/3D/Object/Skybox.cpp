#include "Skybox.h"
#include "SkyboxCloudShader.h"
#include "SkyboxShaders.h"
#include <System/MemoryManagement.h>
#include "Graphics/Framebuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureCube.h"
#include "Graphics/Variables.h"
#include "../../PostProcessing/PostProcessingShader.h"
#include "../Renderer3D.h"
#include "System/Output.h"
#include <Graphics/WrapperFunctions.h>
#include <Essentials/Clock.h>

mat3 m3  = mat3( 0.00f,  0.80f,  0.60f,
                -0.80f,  0.36f, -0.48f,
                -0.60f, -0.48f,  0.64f);
mat3 m3i = mat3(0.00f, -0.80f, -0.60f,
                0.80f,  0.36f, -0.48f,
                0.60f, -0.48f,  0.64f );

float hash1( float n )
{
    return Gum::Maths::fract( n*17.0f*Gum::Maths::fract( n*0.3183099f ) );
}

vec4 noised(vec3 x)
{
    vec3 p = vec3::floor(x);
    vec3 w = vec3::fract(x);
    #if 1
    vec3 u = w*w*w*(w*(w*6.0f-15.0f)+10.0f);
    vec3 du = w*w*30.0f*(w*(w-2.0f)+1.0f);
    #else
    vec3 u = w*w*(3.0-2.0*w);
    vec3 du = 6.0*w*(1.0-w);
    #endif

    float n = p.x + 317.0f*p.y + 157.0f*p.z;
    
    float a = hash1(n+0.0f);
    float b = hash1(n+1.0f);
    float c = hash1(n+317.0f);
    float d = hash1(n+318.0f);
    float e = hash1(n+157.0f);
    float f = hash1(n+158.0f);
    float g = hash1(n+474.0f);
    float h = hash1(n+475.0f);

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    vec3 nn = du * 2.0f * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                    k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                    k3 + k6*u.x + k5*u.y + k7*u.x*u.y);

    return vec4(-1.0f+2.0f*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z), 
                nn.x, nn.y, nn.z);
}


vec4 fbmd_8(vec3 x)
{
  float f = 2.0f;
  float s = 0.65f;
  float a = 0.0f;
  float b = 0.5f;
  vec3  d = vec3(0.0f);
  mat3  m = mat3(1.0f,0.0f,0.0f,
                  0.0f,1.0f,0.0f,
                  0.0f,0.0f,1.0f);
  for( int i=0; i<8; i++ )
  {
      vec4 n = noised(x);
      a += b*n.x;          // accumulate values		
      if(i < 4)
          d += m * b * vec3(n.y, n.z, n.w);      // accumulate derivatives
      b *= s;
      x = m3 * x * f;
      m = m3i * f * m;
  }
  return vec4(a, d.x, d.y, d.z);
}

SkyBox::SkyBox(Mesh *mesh, vec3 *SunDirection, std::string name)
  : Object3D(mesh, name)
{
  initShaders();
	this->sunDir = SunDirection;
  this->cloudNoise3D = nullptr;
  this->cloudNoise2D = nullptr;

	//Create and add Properties
  addInstance();

  //TODO
  /*cloudNoise2D = new Texture2Df("cloudNoise2D", ivec2(32,32), [](const ivec2& uv) {
      vec4 col = vec4(1,0,1,1);
      return rgba(col.x, col.y, col.z, col.w);
  });*/

  pTexture = new EnvironmentMapf(ivec2(1024, 1024), "Skybox");
  pIrradianceMap = new EnvironmentMapf(ivec2(32, 32), "IrradianceMap");
  pPreFilterMap = new EnvironmentMapf(ivec2(128, 128), "PreFilterMap");
  pPreFilterMap->createMipmaps();
  pPreFilterMap->clampToEdge();
  pPreFilterMap->setFiltering(Texture::FilteringType::LINEAR_MIPMAP_LINEAR);

  pBRDFFramebuffer = new Framebuffer(ivec2(512, 512));
  pBRDFFramebuffer->addTextureAttachment<float>(0, "SkyboxBRDFLUTMap", 2U);
  pBRDFFramebuffer->getTextureAttachment(0)->clampToEdge();
  pBRDFFramebuffer->getTextureAttachment(0)->setFiltering(Texture::FilteringType::LINEAR);
  pBRDFCanvas = new Canvas(pBRDFFramebuffer->getSize());

  makeCubeMap(nullptr);
  updateTexture();
}

SkyBox::~SkyBox() 
{
  Gum::_delete(pTexture);
  Gum::_delete(pIrradianceMap);
  Gum::_delete(pBRDFFramebuffer);
  Gum::_delete(pPreFilterMap);
  Gum::_delete(pBRDFCanvas);
}

void SkyBox::render()
{
  Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
  //Gum::Graphics::renderWireframe(false);
  if(bRenderSky)
  {
    Gum::Graphics::cullBackside(false);
    //cloudNoise2D->bind(0);
    getShaderProgram()->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
    getShaderProgram()->loadUniform("sunDir", *this->sunDir);
    //getShaderProgram()->loadUniform("time", Time::getTime());
    renderMesh();
    //cloudNoise2D->unbind(0);
    Gum::Graphics::cullBackside(true);

    //Load time only if clouds are enabled
    if(bRenderClouds)
    {
      ShaderProgram* current = ShaderProgram::getCurrentlyBoundShader();
      CloudShader->use();
      CloudShader->loadUniform("camPos", Camera::getActiveCamera()->getPosition());
      CloudShader->loadUniform("camDir", Camera::getActiveCamera()->getViewDirection());
      CloudShader->loadUniform("sunDir", *this->sunDir);
      CloudShader->loadUniform("time", Time::getTime() * 0.5f);
      
      cloudNoise3D->bind(0);
      Renderer::getActiveRenderer()->getRenderCanvas()->render();
      cloudNoise3D->unbind(0);
      current->use();
    }
  }
  else
  {
    ShaderProgram* current = ShaderProgram::getCurrentlyBoundShader();
    HDRShader->use();
    Gum::Graphics::cullBackside(false);
    pTexture->bind(0);
    HDRShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
    renderMesh();
    pTexture->unbind(0);
    Gum::Graphics::cullBackside(true);
    if(current != nullptr)
      current->use();
  }
  Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
}

void SkyBox::updateTexture()
{
  Gum::Graphics::cullBackside(false);
  Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
  Gum::Graphics::renderWireframe(false);
  makeIrradianceMap();
  makePrefilterMap();
  makeBRDFMap();
  Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
  Gum::Graphics::cullBackside(true);
}

void SkyBox::setTexture(Texture *tex)
{
	if(tex->getType() == Texture::TEXTUREHDR || tex->getType() == Texture::TEXTURE2D)
	{
    makeCubeMap(tex);
    pTexture->createMipmaps();
    updateTexture();
	}
	else if(tex->getType() == Texture::TEXTURECUBE)
	{
    Gum::Output::warn("Skybox: Texturecube not implemented yet!");
    //pTexture = (TextureCube*)tex;
    updateTexture();
	}
	else
	{
		Gum::Output::error("Skybox: invalid Texture set!");
	}
}

void SkyBox::makeBRDFMap()
{
  Clock<> benchmarkClock;
	pBRDFFramebuffer->bind();
  pBRDFFramebuffer->clear(Framebuffer::ClearFlags::COLOR);
	BRDFMapShader->use();
	pBRDFCanvas->render();
	BRDFMapShader->unuse();
  pBRDFFramebuffer->unbind();
  Gum::Output::debug("Skybox: Creating BRDF map took " + std::to_string(benchmarkClock.getPassedTimeInMicros()) + "us");
}

void SkyBox::makePrefilterMap()
{
  Clock<> benchmarkClock;
  ivec2 res = pPreFilterMap->getSize();
  PreFilteredMapShader->use();
  pTexture->bind(0);
  unsigned short maxMipLevels = 5;
  for(unsigned short mip = 0; mip < maxMipLevels; ++mip)
  {
    // resize framebuffer according to mip-level size.
    pPreFilterMap->setActiveMipmapLevel(mip);
    pPreFilterMap->getFramebuffer()->setSize(vec2(res) * std::pow(0.5f, mip));

    float roughness = (float)mip / (float)(maxMipLevels - 1);
    pPreFilterMap->render([this, roughness]() {
      PreFilteredMapShader->loadUniform("roughness", roughness);
      PreFilteredMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
      PreFilteredMapShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
      PreFilteredMapShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());

      renderMesh();
    });
  }
  pTexture->unbind(0);
  PreFilteredMapShader->unuse();

  pPreFilterMap->setActiveMipmapLevel(0);
  pPreFilterMap->getFramebuffer()->setSize(res);

  Gum::Output::debug("Skybox: Creating prefilter map took " + std::to_string(benchmarkClock.getPassedTimeInMicros()) + "us");
}

void SkyBox::makeIrradianceMap()
{
  Clock<> benchmarkClock;

  pTexture->bind(0);
  IrradianceMapShader->use();
  IrradianceMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
  pIrradianceMap->render([this]() {
    IrradianceMapShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    IrradianceMapShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    renderMesh();
  });
  IrradianceMapShader->unuse();
  pTexture->unbind(0);
  Gum::Output::debug("Skybox: Creating irradiance map took " + std::to_string(benchmarkClock.getPassedTimeInMicros()) + "us");
}

void SkyBox::makeCubeMap(Texture* texture)
{
  Gum::Graphics::cullBackside(false);
  Gum::Graphics::disableFeature(Gum::Graphics::Features::DEPTH_TESTING);
  Gum::Graphics::renderWireframe(false);
  if(texture != nullptr)
    texture->bind(0);

  HDRToCubeMapShader->use();
  HDRToCubeMapShader->loadUniform("gradiant", (int)(texture == nullptr));
  HDRToCubeMapShader->loadUniform("SunDirection", *this->sunDir);
  pTexture->render([this]() {
    HDRToCubeMapShader->loadUniform("transformationMatrix", getInstance(0)->getMatrix());
    HDRToCubeMapShader->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
    HDRToCubeMapShader->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    renderMesh();
  });
  HDRToCubeMapShader->unuse();
  TextureCube::unbindGlobal(0);
  Gum::Graphics::enableFeature(Gum::Graphics::Features::DEPTH_TESTING);
  Gum::Graphics::cullBackside(true);
}


//
// Getter
//
TextureCubef* SkyBox::getTexture()             { return pTexture; }
TextureCubef* SkyBox::getIrradianceMap()       { return pIrradianceMap; }
TextureCubef* SkyBox::getPreFilterMap()        { return pPreFilterMap; }
Texture2D* SkyBox::getBRDFConvMap()            { return (Texture2D*)pBRDFFramebuffer->getTextureAttachment(0); }


//
// Setter
//
void SkyBox::renderSky(bool rendersky)        { this->bRenderSky = rendersky; updateTexture(); }
void SkyBox::renderClouds(bool renderclouds)
{ 
  if(renderclouds && cloudNoise3D == nullptr)
  {
    cloudNoise3D = new Texture3Df("cloudNoise3D", ivec3(128,128,128), [](const ivec3& uv) {
      vec4 col = fbmd_8(uv) * 255.0f;
      //Gum::Output::print(col);
      return rgba(col.x, col.y, col.z, col.w);
    });
  }
  this->bRenderClouds = renderclouds;
}

void SkyBox::initShaders()
{
  if(SkyShader == nullptr)
  {
    Shader* skyVertexShader = new Shader(SkyboxVertexShader, Shader::TYPES::VERTEX_SHADER);

    SkyShader = new ShaderProgram("SkyShader", true);
    SkyShader->addShader(skyVertexShader);
    SkyShader->addShader(new Shader(SkyboxFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    SkyShader->build();
    
    HDRShader = new ShaderProgram("HDRShader", true);
    HDRShader->addShader(skyVertexShader);
    HDRShader->addShader(new Shader(HDRFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    HDRShader->build();

    CloudShader = new ShaderProgram("CloudShader", true);
    CloudShader->addShader(new Shader(PostProcessingVertexShader, Shader::TYPES::VERTEX_SHADER));
    CloudShader->addShader(new Shader(CloudFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    CloudShader->build();


    HDRToCubeMapShader = new ShaderProgram("HDRToCubeMapShader", true);
    HDRToCubeMapShader->addShader(skyVertexShader);
    HDRToCubeMapShader->addShader(new Shader(SkyboxHDRToCubeFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    HDRToCubeMapShader->build();
    HDRToCubeMapShader->addTexture("hdrTexture", 0);

    
    IrradianceMapShader = new ShaderProgram("IrradianceMapShader", true);
    IrradianceMapShader->addShader(skyVertexShader);
    IrradianceMapShader->addShader(new Shader(SkyboxIrradianceFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    IrradianceMapShader->build();
    IrradianceMapShader->addTexture("cubeMap", 0);

    
    PreFilteredMapShader = new ShaderProgram("PreFilteredMapShader", true);
    PreFilteredMapShader->addShader(skyVertexShader);
    PreFilteredMapShader->addShader(new Shader(SkyboxPrefilterFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    PreFilteredMapShader->build();
    PreFilteredMapShader->addTexture("cubeMap", 0);

    
    BRDFMapShader = new ShaderProgram("BRDFMapShader", true);
    BRDFMapShader->addShader(new Shader(SkyboxBRDFVertexShader, Shader::TYPES::VERTEX_SHADER));
    BRDFMapShader->addShader(new Shader(SkyboxBRDFFragmentShader, Shader::TYPES::FRAGMENT_SHADER));
    BRDFMapShader->build();
    BRDFMapShader->addTexture("cubeMap", 0);
  }

  pShader = SkyShader;
}