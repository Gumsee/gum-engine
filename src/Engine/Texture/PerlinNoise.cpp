#include "PerlinNoise.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <Graphics/WrapperFunctions.h>

PerlinNoise::PerlinNoise(ivec2 size, int seed, std::string name)
  : iSeed(seed)
{
    //Set properties
    this->sName = name;
	this->v2Size = size;

    repeat();
    setFiltering(FilteringType::LINEAR);
    this->regenerate();
    this->bLoaded = true;
}

PerlinNoise::~PerlinNoise() {}


double PerlinNoise::Noise(int seed, int x, int y) 
{
    // int n = x + y * 57;
    // n = (n << 13) ^ n;

    // srand(seed);
    // int a = rand(), b = rand(), c = rand();
    // int t = (n * (n * n * a + b) + c) & 0x7fffffff;
    // return 1.0 - (double)(t)/1073741824.0;
   // srand(seed + x * y);
    return (rand() % 32768) / 32768.0;
}

double PerlinNoise::SmoothedNoise(int seed, double x, double y)
{
    //get fractional part of x and y
    double fractX = x - int(x);
    double fractY = y - int(y);

    //wrap around
    int x1 = (int(x) + (int)this->v2Size.x) % (int)this->v2Size.x;
    int y1 = (int(y) + (int)this->v2Size.y) % (int)this->v2Size.y;

    //neighbor values
    int x2 = (x1 + (int)this->v2Size.x - 1) % (int)this->v2Size.x;
    int y2 = (y1 + (int)this->v2Size.y - 1) % (int)this->v2Size.y;

    //smooth the noise with bilinear interpolation
    double value = 0.0;
    value += fractX     * fractY     * noise[y1][x1];
    value += (1 - fractX) * fractY     * noise[y1][x2];
    value += fractX     * (1 - fractY) * noise[y2][x1];
    value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

    return value;
}

double PerlinNoise::turbulence(int seed, double x, double y, double size)
{
  double value = 0.0, initialSize = size;

  while(size >= 1)
  {
    value += SmoothedNoise(seed, x / size, y / size) * size;
    size /= 2.0;
  }

  return(128.0 * value / initialSize);
}

void PerlinNoise::regenerate()
{
    noise.clear();
    for(int x = 0; x < v2Size.x; x++)
    {
        std::vector<double> currentNoise;
        for(int y = 0; y < v2Size.y; y++)
        {
            currentNoise.push_back(Noise(iSeed, x, y));
        }
        noise.push_back(currentNoise);
    }

    for(int x = 0; x < v2Size.x; x++)
    {
        for(int y = 0; y < v2Size.y; y++)
        {
            float val = turbulence(iSeed, x, y, 256);//16*Noise(seed, x,y) + 8*Noise(seed + 3, 2*x,2*y) + 4*Noise(seed + 23, 4*x,4*y) + 2*Noise(seed + 154, 8*x,8*y) + Noise(seed + 67, 16*x,16*y);
            //float val = noise[x][y] * 255;
            setPixel(x, y, color(val, val, val, 255));
        }
    }

    updateImage();
}