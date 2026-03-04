#pragma once
#include <Graphics/Texture3D.h>
#include <vector>

class PerlinNoise3D : public Texture3D
{
private:
    int iSeed;

    std::vector<std::vector<std::vector<double> > > noise;

    double Noise(int seed, int x, int y, int z);
    double SmoothedNoise(int seed, double x, double y, double z);
    double turbulence(int seed, double x, double y, double z, double size);

public:
    PerlinNoise3D(ivec3 size, int seed, std::string name);
    ~PerlinNoise3D();

    void regenerate();
};