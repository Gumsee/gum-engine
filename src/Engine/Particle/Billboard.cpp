#include "Billboard.h"
#include <Maths/MatrixFunctions.h>
#include <Essentials/Time.h>
#include "../Rendering/Camera.h"
#include "../Rendering/Renderer.h"
#include "Graphics/Graphics.h"
#include "Graphics/Variables.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/VertexBufferObject.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"

Billboard::Billboard(vec3 Position)
{
	  //this->terrain = terrain;
	  this->Position = Position;
    this->Rotation = vec3(0.0f);
    //this->Scale = vec2(30) / Gum::Window->getRenderQuadSize(); // 30x30 pixels
    this->Scale = vec2(0.1f, 0.1f * Renderer::getActiveRenderer()->getFramebuffer()->getAspectRatio()); //10% of the screen
	  this->tex = nullptr;

    pVAO = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP);
    VertexBufferObject<float> vbo;
    vbo.setData(vertices, Gum::Graphics::DataState::STATIC);
    pVAO->addAttribute(&vbo, 0, 3, Gum::Graphics::Datatypes::FLOAT, 0);
    pVAO->setVertexCount((unsigned int)vertices.size());
}

Billboard::~Billboard() 
{
    Gum::_delete(pVAO);
}

void Billboard::render(ShaderProgram *shader)
{
	if(vec3::distance(Camera::getActiveCamera()->getPosition(), getPosition()) < 4000)
	{
    shader->loadUniform("billboardCenter", Position);
    shader->loadUniform("billboardSize", Scale);
    shader->loadUniform("fixedSize", bFixedSize);

    Gum::Graphics::disableFeature(Gum::Graphics::Features::CULL_FACE);
    
    //if (Transparency) { glBlendFunc(GL_SRC_ALPHA, GL_ONE); }
    //else              { glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
    //glDepthMask(false);

		if(tex != nullptr) 
      tex->bind(0);
    pVAO->bind();
    pVAO->render(1);
    pVAO->unbind();
		if(tex != nullptr) 
      tex->unbind(0);

    //glDepthMask(true);
    //glDisable(GL_BLEND);
    Gum::Graphics::enableFeature(Gum::Graphics::Features::CULL_FACE);
	}
}

bool Billboard::operator<(Billboard& that) { return this->getcameradistance() > that.getcameradistance(); } // Sort in reverse order : far Billboards drawn first.


//
//Setter
//
void Billboard::setTexture(Texture *tex)     { this->tex = tex; }
void Billboard::setPosition(vec3 pos)        { this->Position = pos; }
void Billboard::setScale(vec2 scale)         { this->Scale = scale; }
void Billboard::setPixelSize(vec2 size)      { this->Scale = size / Renderer::getActiveRenderer()->getRenderCanvas()->getSize(); } 
void Billboard::useFixedSize(bool fixedsize) { this->bFixedSize = fixedsize; }

//
//Getter
//
vec3 Billboard::getPosition()                { return Position; }
mat4* Billboard::getTransformationMatrix()   { return &TransMatrix; }
float Billboard::getcameradistance()         { return vec3::distance(Position, Camera::getActiveCamera()->getPosition()); }
Texture* Billboard::getTexture()             { return tex; }