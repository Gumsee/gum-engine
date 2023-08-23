#include "PerlinNoise3D.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <Graphics/WrapperFunctions.h>

PerlinNoise3D::PerlinNoise3D(ivec3 size, float seed, std::string name)
{
    //Set properties
    this->sName = name;
	this->iType = TEXTURE3D;
	this->v3Size = size;
    this->seed = seed;


    //(generate texel code omitted)
    bind(0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    unbind(0);

    regenerate();
    this->bLoaded = true;
}

PerlinNoise3D::~PerlinNoise3D()
{

}


double PerlinNoise3D::Noise(int seed, int x, int y, int z) 
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

double PerlinNoise3D::SmoothedNoise(int seed, double x, double y, double z)
{
    //get fractional part of x and y
    double fractX = x - int(x);
    double fractY = y - int(y);
    double fractZ = z - int(z);

    //wrap around
    int x1 = (int(x) + (int)this->v3Size.x) % (int)this->v3Size.x;
    int y1 = (int(y) + (int)this->v3Size.y) % (int)this->v3Size.y;
    int z1 = (int(z) + (int)this->v3Size.z) % (int)this->v3Size.z;

    //neighbor values
    int x2 = (x1 + (int)this->v3Size.x - 1) % (int)this->v3Size.x;
    int y2 = (y1 + (int)this->v3Size.y - 1) % (int)this->v3Size.y;
    int z2 = (z1 + (int)this->v3Size.z - 1) % (int)this->v3Size.z;

    //smooth the noise with bilinear interpolation
    double value = 0.0;
    value += fractX     * fractY     * noise[y1][x1][z1];
    value += (1 - fractX) * fractY     * noise[y1][x2][z1];
    value += (1 - fractX) * fractY     * noise[y1][x2][z2];

    value += fractX     * (1 - fractY) * noise[y2][x1][z1];
    value += (1 - fractX) * (1 - fractY) * noise[y2][x2][z1];
    value += (1 - fractX) * (1 - fractY) * noise[y2][x2][z2];

    return value;
}

double PerlinNoise3D::turbulence(int seed, double x, double y, double z, double size)
{
  double value = 0.0, initialSize = size;

  while(size >= 1)
  {
    value += SmoothedNoise(seed, x / size, y / size, z / size) * size;
    size /= 2.0;
  }

  return(128.0 * value / initialSize);
}

void PerlinNoise3D::regenerate()
{
    noise.clear();
    for(int x = 0; x < v3Size.x; x++)  // X Iteration
    {
        std::vector<std::vector<double> > currentVector;
        for(int y = 0; y < v3Size.y; y++)  // Y Iteration
        {
            std::vector<double> currentNoise;
            for(int z = 0; z < v3Size.z; z++)  // Z Iteration
            {
                currentNoise.push_back(Noise(seed, x, y, z));
            }
            currentVector.push_back(currentNoise);
        }
        noise.push_back(currentVector);
    }

    std::vector<float> pixels;

    for(int x = 0; x < v3Size.x; x++)
    {
        for(int y = 0; y < v3Size.y; y++)
        {
            for(int z = 0; z < v3Size.z; z++)
            {
                float val = turbulence(seed, x, y, z, 256);
                val = rand() % 256;
                val /= 256;
                pixels.push_back(val);
                val = rand() % 256;
                val /= 256;
                pixels.push_back(val);
                val = rand() % 256;
                val /= 256;
                pixels.push_back(val);
            }
        }
    }

    //glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, size.x, size.y, size.z, 0, GL_RGB, GL_FLOAT, &pixels[0]);
}