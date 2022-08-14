#pragma once
#include <Maths/vec.h>

struct ParticleProperties
{
    bool Transparency = false;
    bool RandomLifetime = false;
    int NumberOfRows = 1;
    int NumberOfCollumns = 1;
	float lifetime = 1;
    float killtime = 1;
    int maxamount = 1000;
    int amount = 10;
    vec3 RandomPositionOffset = vec3(0,0,0);
};