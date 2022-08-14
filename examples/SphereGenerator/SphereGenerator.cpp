#include <Engine.h>
#include <iostream>
#include "../Examples.h"

int currentInstance = 0;
vec3 lastPos;
bool setBlock(vec3 pos, Object *obj)
{
    if(pos != lastPos)
    {
        obj->setPosition(pos, currentInstance);
        obj->addInstance();
        currentInstance++;
        lastPos = pos;
        return true;
    }
    return false;
}

/* pos is a Vector containing the x,y,z of the center block of the sphere
 * block is the block to make the sphere out of
 * radius is the radius of the sphere
 * filled is whether the sphere is solid or hollow
 */
int makeSphere(vec3 pos, Object* obj, double radius, bool half)
{
    int affected = 0; //Number of blocks changed

    radius += 0.5; //I think they do this so the radius is measured from the center of the block
    double radiusSq = radius * radius; //Square of the radius, so we don't need to use square roots for distance calcs
    double radius1Sq = (radius - 1.0) * (radius - 1.0); //Square of the radius of a circle 1 block smaller, for making a hollow sphere

    int ceilRadius = ceil(radius); //Round the radius up
    std::cout << "Radius: " << ceilRadius << std::endl;
    //Loop through x,y,z up to the rounded radius
    for (int x = 0; x <= ceilRadius; x++) 
    {
        for (int y = 0; y <= ceilRadius; y++) 
        {
            for (int z = 0; z <= ceilRadius; z++) 
            {
                double dSq = (x*x + y*y + z*z); //Gets the square of the distance from the center (x*x + y*y + z*z). Again using squares so we don't need to square root

                //If the distance to this point is greater than the radius, skip it (this is what makes this whole thing make a sphere, instead of a cube)
                if (dSq > radiusSq) 
                {
                    continue;
                }

                //If sphere should be hollow, and the point is within the sphere, but also within the 1-smaller sphere, skip it
                if ((dSq < radius1Sq) || ((((x+1)*(x+1) + y*y + z*z) <= radiusSq) && ((x*x + (y+1)*(y+1) + z*z) <= radiusSq) && ((x*x + y*y + (z+1)*(z+1)) <= radiusSq)))
                {
                    continue;
                }
                
                //Place the block in every +/- direction around the center
                if (setBlock(pos + vec3( x,  y,  z), obj)) { affected++; }
                if (setBlock(pos + vec3(-x,  y,  z), obj)) { affected++; }
                if (setBlock(pos + vec3( x,  y, -z), obj)) { affected++; }
                if (setBlock(pos + vec3(-x,  y, -z), obj)) { affected++; }
                if(!half)
                {
                    if (setBlock(pos + vec3( x, -y,  z), obj)) { affected++; }
                    if (setBlock(pos + vec3(-x, -y,  z), obj)) { affected++; }
                    if (setBlock(pos + vec3( x, -y, -z), obj)) { affected++; }
                    if (setBlock(pos + vec3(-x, -y, -z), obj)) { affected++; }
                }
            }
        }
    }

    //Return number of blocks added
    return affected;
}


World* Examples::createSphereGeneratorExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

    //Defaults
    int radius = 3;
    bool half = false;


    pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
    pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
    pWorld->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
    pWorld->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));

    Object *obj = new Object(Mesh::generateCube(vec3(0.5)), "test");

    
    makeSphere(vec3(0,0,0), obj, radius, half);

    pWorld->getObjectManager()->addObject(obj);


	return pWorld;
}
