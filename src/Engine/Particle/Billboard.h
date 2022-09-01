#pragma once
#include <OpenGL/Texture.h>
#include <OpenGL/ShaderProgram.h>

class Billboard
{
private:
	vec3 Position;
	vec3 Rotation;
	vec2 Scale;
	bool Transparency = false;
    bool bFixedSize = true;

	GLuint VAO;
	GLuint VertexPositions;
	int VertexCount = 0;

	Texture *tex;
	mat4 TransMatrix;

public:
	Billboard(vec3 Position);
	~Billboard();

	void render(ShaderProgram *shader);
	bool operator<(Billboard& that);


    //Getter
	mat4 *getTransformationMatrix();
	float getcameradistance();
	Texture *getTexture();

    //Setter
	vec3 getPosition();
	void setPosition(vec3 pos);
	void setScale(vec2 scale);
	void setPixelSize(vec2 size); //Only makes sense in fixed mode
	void setTexture(Texture *tex);
    void useFixedSize(bool fixedsize);
};