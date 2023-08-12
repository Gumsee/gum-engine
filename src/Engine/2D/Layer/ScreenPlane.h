#pragma once
#include "Layer.h"
#include <OpenGL/VertexArrayObject.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Renderable.h>

class ScreenPlane : public Layer
{
private:
    inline static const std::vector<float> afSkyVertices {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,
    };

    inline static const std::vector<unsigned int> aiSkyIndices {
        0, 1, 2, 2, 3, 0
    };
	inline static VertexArrayObject* pVertexArrayObject = nullptr;

    unsigned int iNumLayers;
    ShaderProgram* pParallaxSkyShader;

public:
    ScreenPlane();
    ~ScreenPlane();

    void render() override;

    void addLayer();
};