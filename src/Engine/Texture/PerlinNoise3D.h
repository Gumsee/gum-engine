#pragma once
#include <OpenGL/Texture3D.h>
#include <vector>

class PerlinNoise3D : public Texture3D
{
private:
    int maxPrimeIndex = 10;
    int numOctaves = 9;
    float persistence = 0.65;
    int primeIndex = 3;
    float seed;

    std::vector<std::vector<std::vector<double> > > noise;

    double Noise(int seed, int x, int y, int z);
    double SmoothedNoise(int seed, double x, double y, double z);
    double turbulence(int seed, double x, double y, double z, double size);

public:
    PerlinNoise3D(ivec3 size, float seed, std::string name);
    ~PerlinNoise3D();

    void regenerate();
};