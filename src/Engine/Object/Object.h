#pragma once
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/VertexArrayObject.h>
#include <OpenGL/Texture.h>
#include <Essentials/Settings.h>
#include <Essentials/BoundingBox.h>
#include "../Material/Material.h"
#include "../Loaders/ObjectLoader.h"
#include "../General/Animation.h"
#include "Transformation.h"

#define	SPHERE		0
#define	BOX_SHAPE	1
#define	CONE		2
#define	CYLINDER	3
#define	PLANE		4
#define	SOFTBODY	5
#define	CONVEX		6
#define CAPSULE		7

#define OBJECT_TYPE_FILE 	0
#define OBJECT_TYPE_TERRAIN 1

class Object; //Predefine
class World;

struct Instance : public Transformation
{
	bool Copyable = true;			//Copy the properties or use existing pointer
	bool ismouseon = false;
	bool renderOutline = false;
	Animation *pAnimation;
	Object *parentObject = nullptr;
	unsigned int id = 0;				//Global ID

	int CollisionObjectType = -1;						//Collision Type
	float CollisionObjectMass = 0.0f;					//Collision Mass
	bool CollisionObjectActivity = false;				//Collision isActive?
	vec3 CollisionObjectSpecial = vec3(0.0f);	//Collision SpecialVar?

	void BulletClickCallBackFunction() { ismouseon = true; }
};

struct ObjectProperties
{
	//General
	bool hasbackface = true;		//Render only one side?
	bool inverseCulling = false;	//Show insides?
	bool RendererdToShadowMap = true;
	bool renderTessellated = false;
	bool isInstanced = false;
	int ObjectType = 0; 			//TYPE
	float furthestAwayPoint = 0.0f;
	std::string Name;				//Object Name
	void* customData = nullptr;


	//Technical	(OpenGL)
	Mesh *pMesh;
	
	//Outline
	bool outline = false;
	vec3 outlineColor = vec3(1);
	float outlineThickness = 4;
};

class Object
{
private:
	void createIndividualColor(Instance *instance);
	bbox3 bBoundingBox;

protected:
	Object();

	Material *pMaterial;
	ShaderProgram *pShader;
	ObjectLoader *pObjectLoader;
	ObjectProperties *pProperties;
	VertexArrayObject* pVertexArrayObject;
    VertexBufferObject<mat4>* pTransMatricesVBO;
    VertexBufferObject<Vertex>* pVertexVBO;
    VertexBufferObject<vec3>* pIndividualColorsVBO;

	std::vector<Instance*> vInstances;
	std::vector<mat4> vTransforms;
	std::vector<vec3> vIndividualColors;


	void getFurthestAwayPoint(Instance *inst);
	void load();

public:
	Object(std::string ModelFilePath, std::string name);
	Object(ObjectProperties *properties, std::string name);
	Object(Mesh *mesh, std::string name);
	virtual ~Object();

	virtual void render();
	void renderMesh();

	void prepareRender();
	void finishRender();
	void generateBoundingBox(Instance *inst);

	void reload();

	void writeObjectInfoToFile(std::string filename);

	void setActivation(bool state, int index = 0);
	void setActivation(bool state, Instance *inst);
	void forceActivation(bool state, int index = 0);
	void addPhysics(int type, bool isActive, float mass, Instance *instance, World* world, vec3 special = vec3(0,0,0));

	
	void addInstance();
	void addInstance(Instance *inst);
	void operator++();

	void RenderToShadowMap(bool var);
	void setMaterial(Material *mat);
	void setProperties(ObjectProperties *properties);
	void setInstance(Instance *instance, int index = 0);
	void setShader(ShaderProgram *shader);
	void setName(std::string name);
	void setTessellatedShader(bool val);
	void setTranformationMatrix(mat4 matrix, int index = 0);
	void setOutline(bool DrawOutline);
	void setOutlineThickness(float thickness);
	void setOutlineColor(vec3 outlineColor);

	//Throughput
	void setPosition(vec3 pos, int index = 0);
	void setRotation(quat rot, int index = 0);
	void setRotation(vec3 rot, int index = 0);
	void setScale(vec3 scale, int index = 0);
	void setScale(float scale, int index = 0);
	void increasePosition(vec3 pos, int index = 0);
	void increaseRotation(quat rot, int index = 0);
	void increaseRotation(vec3 rot, int index = 0);
	void increaseScale(vec3 scale, int index = 0);

	btRigidBody* 		getRigidBody(int index = 0);
	ShaderProgram*		getShader();
	mat4*		    	getTransformationMatrix(int index = 0);
	std::string 		getName();
	ObjectProperties*	getProperties();
	Instance* 			getInstance(int index = 0);
	GLuint 				getShaderID();
	Material* 			getMaterial();
	bool 				isRendererdToShadowMap();
	bool 				isMouseOn(int index = 0);
	bool				isCollidingWithRay(vec3 ray, int index = 0);
	int 				numInstances();
    VertexArrayObject*  getVertexArrayObject();

	//Throughput
	vec3 			    getPosition(int index = 0);
	quat 			    getRotation(int index = 0);
	vec3 			    getScale(int index = 0);
};
