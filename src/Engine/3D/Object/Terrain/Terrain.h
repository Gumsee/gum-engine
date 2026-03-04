#pragma once
#include <Primitives/Mesh.h>
#include <Graphics/Texture2D.h>

class Terrain : public Mesh
{
private:
    ivec2 v2Resolution;
    ivec2 v2Size;
    float fAmplitude;
	int iRandomNumber;

	int OCTAVES = 3;
	float ROUGHNESS = 0.3f;
	int SmoothenFactor = 0;

	std::vector<std::vector<float> > heights;


	vec3 calculateNormal(int x, int z);
	vec3 calculateNormalTex(int x, int z, Texture2D* heightMap);

	void smoothenHeights();
	float generateHeight(int x, int z);
	float getInterpolatedNoise(float x, float z);
	float getSmoothNoise(int x, int z);
	double interpolate(double x, double y, double a) const;
	double Noise(int x, int y) const;

	float getHeightFromIndex(int x, int z);

public:
	Terrain(const vec2& dimensions, const ivec2& resolution, const float& amplitude, const int& seed);
	~Terrain();

	//void setGround(float Height);
	void setSmoothFactor(int factor)
	{
		this->SmoothenFactor = factor;
	}

	vec2 getSize();
	ivec2 getResolution();
	float getHeight(float x, float z);
};