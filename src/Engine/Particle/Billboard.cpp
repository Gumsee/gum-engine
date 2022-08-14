#include "Billboard.h"
#include <Maths/MatrixFunctions.h>
#include <Essentials/FPS.h>
#include "../General/Camera.h"
#include "../General/Renderer3D.h"
#include "Essentials/Window.h"

Billboard::Billboard(vec3 Position, Renderer3D* renderer)
{
	pRenderer = renderer;
	//this->terrain = terrain;
	this->Position = Position;
    this->Rotation = vec3(0,0,0);
    //this->Scale = vec2(30) / Gum::Window->getRenderQuadSize(); // 30x30 pixels
    this->Scale = vec2(0.1f * pRenderer->getAspectRatio(), 0.1f); //10% of the screen
	this->tex = nullptr;

	std::vector<float> vertices =
	{
		-0.5, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, 0.5, 0,
		0.5, 0.5, 0,
		-0.5,-0.5, 0,
		0.5, -0.5, 0
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VertexPositions);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VertexPositions);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	VertexCount = vertices.size();
}

Billboard::~Billboard() {}

void Billboard::render(ShaderProgram *shader)
{
	if(vec3::distance(Camera::ActiveCamera->getPosition(), getPosition()) < 4000)
	{
        shader->LoadUniform("transformationMatrix", mat4());
        shader->LoadUniform("billboardCenter", Position);
        shader->LoadUniform("billboardSize", Scale);
        shader->LoadUniform("fixedSize", bFixedSize);

        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        
        if (Transparency) { glBlendFunc(GL_SRC_ALPHA, GL_ONE); }
        else              { glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
        glDepthMask(false);

        tex->bind(0);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        tex->unbind(0);

        glDepthMask(true);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
	}
}

bool Billboard::operator<(Billboard& that) { return this->getcameradistance() > that.getcameradistance(); } // Sort in reverse order : far Billboards drawn first.


//
//Setter
//
void Billboard::setTexture(Texture *tex)     { this->tex = tex; }
void Billboard::setPosition(vec3 pos)        { this->Position = pos; }
void Billboard::setScale(vec2 scale)         { this->Scale = scale; }
void Billboard::setPixelSize(vec2 size)      { this->Scale = size / pRenderer->getRenderCanvas()->getSize(); } 
void Billboard::useFixedSize(bool fixedsize) { this->bFixedSize = fixedsize; }

//
//Getter
//
vec3 Billboard::getPosition()                { return Position; }
mat4* Billboard::getTransformationMatrix()   { return &TransMatrix; }
float Billboard::getcameradistance()         { return vec3::distance(Position, Camera::ActiveCamera->getPosition()); }
Texture* Billboard::getTexture()             { return tex; }