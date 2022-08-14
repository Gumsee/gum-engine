#pragma once
#include <OpenGL/ShaderProgram.h>

class Grid
{
private:
    ShaderProgram* pShader;

public:
    Grid();
    ~Grid();

    void render();

    void initShader();
};