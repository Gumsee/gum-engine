#pragma once
#include <Graphics/Texture2D.h>
#include <vector>

class PerlinNoise : public Texture2D
{
private:
    int maxPrimeIndex = 10;
    int numOctaves = 9;
    float persistence = 0.65;
    int primeIndex = 3;
    float seed;

    std::vector<std::vector<double> > noise;

    double Noise(int seed, int x, int y);
    double SmoothedNoise(int seed, double x, double y);
    double turbulence(int seed, double x, double y, double size);

public:
    PerlinNoise(ivec2 size, float seed, std::string name);
    ~PerlinNoise();

    void regenerate();
};