#pragma once
#include "Layer.h"
#include <Graphics/VertexArrayObject.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/Renderable.h>

class Background : public Layer
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

public:
    Background(std::string name);
    ~Background();

    void render() override;

    void addLayer();
};