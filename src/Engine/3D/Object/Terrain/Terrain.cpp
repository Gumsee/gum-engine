#include "Terrain.h"
#include <Essentials/Tools.h>
//#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
//#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>

Terrain::Terrain(const vec2& dimensions, const ivec2& resolution, const float& amplitude, const int& seed) 
{  
    this->v2Resolution = resolution;
    this->v2Size = dimensions;
    this->fAmplitude = amplitude;
	
	srand(seed);
	iRandomNumber = rand();
	for (int i = 0; i < v2Resolution.x; i++) 
	{		
        std::vector<float> collumn;
        for (int j = 0; j < v2Resolution.y; j++) 
		{
            collumn.push_back(generateHeight(i, j));
		}
        heights.push_back(collumn);
	}
    smoothenHeights();

	for (int i = 0; i < v2Resolution.x; i++) 
	{
		for (int j = 0; j < v2Resolution.y; j++) 
		{
            Vertex vertex;

			vertex.position.x = (float)i / ((float)v2Resolution.x) * v2Size.x;
			vertex.position.y = heights[i][j];
			vertex.position.z = (float)j / ((float)v2Resolution.y) * v2Size.y;
			
			vertex.textureCoord.x = (float)i / ((float)v2Resolution.x);
			vertex.textureCoord.y = (float)j / ((float)v2Resolution.y);

			vertex.normal = calculateNormal(i, j);
            vec3 c1 = vec3::cross(vertex.normal, vec3(0.0, 0.0, 1.0));
            vec3 c2 = vec3::cross(vertex.normal, vec3(0.0, 1.0, 0.0));
            vertex.tangent = (c1.length() > c2.length()) ? c1 : c2;

			addVertex(vertex);
		}
	}

	for (int gz = 0; gz < v2Resolution.y - 1; gz++) {
		for (int gx = 0; gx < v2Resolution.x - 1; gx++) {
			int topLeft     = (gz * v2Resolution.y) + gx;
			int topRight    = topLeft + 1;
			int bottomLeft  = ((gz + 1) * v2Resolution.y) + gx;
			int bottomRight = bottomLeft + 1;
			addIndex(bottomRight);
			addIndex(bottomLeft);
			addIndex(topRight);

			addIndex(topRight);
			addIndex(bottomLeft);
			addIndex(topLeft);
		}
    }
}

Terrain::~Terrain() 
{
}

vec3 Terrain::calculateNormal(int x, int z)
{
	float HeightL, HeightR, HeightD, HeightU;
	HeightL = generateHeight(x - 1, z);
	HeightR = generateHeight(x + 1, z);
	HeightD = generateHeight(x, z - 1);
	HeightU = generateHeight(x, z + 1);
	vec3 normal = vec3(HeightL - HeightR, HeightD - HeightU, 2.0f);
	normal = vec3::normalize(normal);
    //normal = vec3(0,1, 0);
	return normal;
}

vec3 Terrain::calculateNormalTex(int x, int z, Texture2D* heightMap)
{
	float HeightL, HeightR, HeightD, HeightU;
	HeightL = heightMap->getHeightMapPixel(x - 1, z) * this->fAmplitude;
	HeightR = heightMap->getHeightMapPixel(x + 1, z) * this->fAmplitude;
	HeightD = heightMap->getHeightMapPixel(x, z - 1) * this->fAmplitude;
	HeightU = heightMap->getHeightMapPixel(x, z + 1) * this->fAmplitude;
	vec3 normal = vec3(HeightL - HeightR, 2.0f, HeightD - HeightU);
	normal = vec3::normalize(normal);
	return normal;
}

float Terrain::getHeightFromIndex(int x, int z)
{
	if(x < 0 || x > v2Resolution.x - 1 || 
       z < 0 || z > v2Resolution.y - 1)
		return 1.0f;
    return heights[x][z];
}

float Terrain::getHeight(float x, float z)
{
	float gridSquareSizeX = v2Size.x / ((float)v2Resolution.x - 1);
	float gridSquareSizeZ = v2Size.y / ((float)v2Resolution.y - 1);
	int gridX = (int)floor(x / gridSquareSizeX);
	int gridZ = (int)floor(z / gridSquareSizeZ);
	if (gridX >= v2Resolution.x - 1 || gridZ >= v2Resolution.y - 1 || gridX < 0 || gridZ < 0)
		return 0;
        
	float xCoord = std::fmod(x, gridSquareSizeX) / gridSquareSizeX;
	float zCoord = std::fmod(z, gridSquareSizeZ) / gridSquareSizeZ;
    
	if (xCoord <= (1 - zCoord))
		return Tools::barryCentric(vec3(0, heights[gridX][gridZ], 0), vec3(1, heights[gridX + 1][gridZ], 0), vec3(0, heights[gridX][gridZ + 1], 1), vec2(xCoord, zCoord));
	else
		return Tools::barryCentric(vec3(1, heights[gridX + 1][gridZ], 0), vec3(1, heights[gridX + 1][gridZ + 1], 1), vec3(0, heights[gridX][gridZ + 1], 1), vec2(xCoord, zCoord));
}


float Terrain::generateHeight(int x, int z) 
{
	float total = 0;
	float d = (float)pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(ROUGHNESS, i) * fAmplitude;
		total += getInterpolatedNoise(x * freq, z * freq) * amp;
	}
	total /= 2;
	return total;
}

float Terrain::getInterpolatedNoise(float x, float z) 
{
	int intX = (int)x;
	int intZ = (int)z;
	double fracX = x - intX;
	double fracZ = z - intZ;

	double v1 = getSmoothNoise(intX, intZ);
	double v2 = getSmoothNoise(intX + 1, intZ);
	double v3 = getSmoothNoise(intX, intZ + 1);
	double v4 = getSmoothNoise(intX + 1, intZ + 1);
	double i1 = interpolate(v1, v2, fracX);
	double i2 = interpolate(v3, v4, fracX);
	return (float)interpolate(i1, i2, fracZ);
}

float Terrain::getSmoothNoise(int x, int z) 
{
	float corners = (Noise(x - 1, z - 1) + Noise(x + 1, z - 1) + Noise(x - 1, z + 1) + Noise(x + 1, z + 1)) / 16.0f;
	float sides = (Noise(x - 1, z) + Noise(x + 1, z) + Noise(x, z - 1) + Noise(x, z + 1)) / 8.0f;
	float center = Noise(x, z) / 4.0f;
	return corners + sides + center;
}

double Terrain::interpolate(double x, double y, double a) const
{
	double negA = 1.0 - a;
	double negASqr = negA * negA;
	double fac1 = 3.0 * (negASqr)-2.0 * (negASqr * negA);
	double aSqr = a * a;
	double fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

	return x * fac1 + y * fac2; //add the weighted factors
}

double Terrain::Noise(int x, int y) const
{
	int n = x + y * (iRandomNumber + 7);
	n = (n << iRandomNumber) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(t) * 0.931322574615478515625e-9;/// 1073741824.0);
}

void Terrain::smoothenHeights()
{
	float k = 0.75;
	/* Rows, left to right */
	for(int x = 1;x < v2Resolution.x; x++)
	{
	    for (int z = 0;z < v2Resolution.y; z++)
		{
			heights[x][z] = getHeightFromIndex(x-1,z) * (1-k) + getHeightFromIndex(x,z) * k;
		}
	}

	/* Rows, right to left*/
	for(int x = v2Resolution.x-2;x < -1; x--)
	{
	    for (int z = 0;z < v2Resolution.y; z++)
		{
			heights[x][z] = getHeightFromIndex(x+1,z) * (1-k) + getHeightFromIndex(x,z) * k;
		}
	}

	/* Columns, bottom to top */
	for(int x = 0;x < v2Resolution.x; x++)
	{
	    for (int z = 1;z < v2Resolution.y; z++)
		{
			heights[x][z] = getHeightFromIndex(x,z-1) * (1-k) + getHeightFromIndex(x,z) * k;	
		}
	}

	/* Columns, top to bottom */
	for(int x = 0;x < v2Resolution.x; x++)
	{
	    for (int z = v2Resolution.y; z < -1; z--)
		{
			heights[x][z] = getHeightFromIndex(x,z+1) * (1-k) +  getHeightFromIndex(x,z) * k;
		}
	}
}

ivec2 Terrain::getResolution() { return v2Resolution; }
vec2 Terrain::getSize()        { return v2Size; }


/*void Terrain::generatePhysics(GumPhysics *physics)
{
	btTransform Transform;
	Transform.setIdentity();
	//Transform.setOrigin(btVector3(getPosition().x, getPosition().y, getPosition().z));
	Transform.setOrigin(btVector3(getPosition().x + terrainData->size.x / 2, getPosition().y , getPosition().z + terrainData->size.y / 2));

	std::vector<float> *HeightfieldData;
	HeightfieldData = new std::vector<float>();

	for(int i = 0; i < heights.size(); i++)
	{
		for(int j = 0; j < heights[i].size(); j++)
		{
			HeightfieldData->push_back(heights[j][i]);
		}
	}

	btHeightfieldTerrainShape *ColShape = new btHeightfieldTerrainShape(heights.size(), heights[0].size(), &(*HeightfieldData)[0], 1, -1024, 1016, 1, PHY_FLOAT, true);
	ColShape->setUseDiamondSubdivision(true);
	ColShape->setLocalScaling(btVector3(terrainData->size.x / heights.size(), 1, terrainData->size.y / heights[0].size()));

	//btVector3 localScaling(size.x, 1, size.y);

	//ColShape->setLocalScaling(localScaling);

	btScalar Mass = 0.0f;

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(Transform);

	delete getInstance()->getBody();

	getInstance()->setBody(new btRigidBody(Mass, myMotionState, ColShape, localInertia));

	//CCD
	getInstance()->getBody()->setCcdMotionThreshold(1e-7);
	getInstance()->getBody()->setCcdSweptSphereRadius(0.50f);

	getInstance()->getBody()->setFriction(0.8);
	getInstance()->getBody()->setHitFraction(0.8);
	getInstance()->getBody()->setRestitution(0.6);

	physics->getWorld()->addRigidBody(getInstance()->getBody());
}*/