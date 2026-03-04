#pragma once
#include <Graphics/Texture2D.h>
#include <vector>

class PerlinNoise : public Texture2D
{
private:
    int iSeed;

    std::vector<std::vector<double> > noise;

    double Noise(int seed, int x, int y);
    double SmoothedNoise(int seed, double x, double y);
    double turbulence(int seed, double x, double y, double size);

public:
    PerlinNoise(ivec2 size, int seed, std::string name);
    ~PerlinNoise();

    void regenerate();
};