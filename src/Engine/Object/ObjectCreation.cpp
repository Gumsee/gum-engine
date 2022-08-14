#include "Object.h"
#include "../Physics/Physics.h"
#include "../Managers/ObjectManager.h"
#include <Essentials/Tools.h>

Object::Object()
{
	pMaterial = new Material();
	pObjectLoader = new ObjectLoader();
	pProperties = new ObjectProperties();
    pVertexArrayObject = new VertexArrayObject();
}

/** Loads the Object files content into memory and loads
 *  @param[in] modelFilePath Full path to object file
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object::Object(std::string modelFilePath, std::string name)
{
	pMaterial = new Material();
	pObjectLoader = new ObjectLoader();
	pProperties = new ObjectProperties();
    pVertexArrayObject = new VertexArrayObject();
	//Set attributes
	this->pShader = nullptr;

	//Open object file
	pObjectLoader->open(ObjectManager::MODEL_ASSETS_PATH + modelFilePath);

	//Create and add Properties
	pProperties->Name = name;

	//Combine all meshes into one
	pProperties->pMesh = new Mesh();
	for(size_t i = 0; i < pObjectLoader->vMeshes.size(); i++)
	{
		pProperties->pMesh->addMesh(pObjectLoader->vMeshes[i]);
	}

	load();
}



/** Copies another object according to its properties
 *  @param[in] properties Object Properties of the object that should be copied
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object::Object(ObjectProperties *properties, std::string name)
{
	pMaterial = new Material();
	pObjectLoader = new ObjectLoader();
	pProperties = new ObjectProperties(*properties);
    pVertexArrayObject = new VertexArrayObject();
	this->pShader = nullptr;
	pProperties->Name = name;
	load();
}


/** Create an object according to a given mesh
 *  @param[in] mesh Mesh containing all necessary information
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object::Object(Mesh *mesh, std::string name)
{
	pMaterial = new Material();
	pObjectLoader = new ObjectLoader();
	pProperties = new ObjectProperties();
    pVertexArrayObject = new VertexArrayObject();
	//Set attributes
	this->pShader = nullptr;

	//Create and add Properties
	pProperties->Name = name;
	pProperties->pMesh = new Mesh(*mesh);


	load();
}


/** Gernerates the Objects Vertex Array and Buffers
 *  It also creates the bounding box
 *  and calculates th furthest away point from the objects origin
 */ 
void Object::load()
{
	if(pProperties->pMesh != nullptr)
	{
		//Binding
        pVertexArrayObject->bind();
        pVertexVBO = new VertexBufferObject<Vertex>();
		pVertexVBO->setData(pProperties->pMesh->getVertexBuffer());
        pVertexArrayObject->addBufferVertices(pVertexVBO);
        
        pTransMatricesVBO = new VertexBufferObject<mat4>();
		//pTransMatricesVBO->setData(vTransforms, GL_STREAM_DRAW);
		pVertexArrayObject->addAttributeMat4(pTransMatricesVBO, 3, GL_FLOAT, 1);

		pIndividualColorsVBO = new VertexBufferObject<vec3>();
		pVertexArrayObject->addAttribute(pIndividualColorsVBO, 10, 3, GL_FLOAT, sizeof(vec3), 0, 1);


        pVertexArrayObject->addElementBuffer(pProperties->pMesh->getIndexBuffer());
        pVertexArrayObject->unbind();


		/*
		Instance *instance = new Instance();
		instance->parentObject = this;
		vInstances.push_back(instance);
		vTransforms.push_back(instance->getMatrix());
		//Get individual ID
		createIndividualColor(instance);*/
		addInstance(new Instance());
		
		generateBoundingBox(vInstances[0]);
		getFurthestAwayPoint(vInstances[0]);
	}
	else
	{
		Gum::Output::warn("ObjectCreation: Mesh is nullptr!");
	}
}


void Object::reload()
{
	load();
}


void Object::createIndividualColor(Instance *instance)
{
	vec3 individualColor;
	individualColor.r = (instance->id & 0x000000FF) >>  0;
	individualColor.g = (instance->id & 0x0000FF00) >>  8;
	individualColor.b = (instance->id & 0x00FF0000) >> 16;
	vIndividualColors.push_back(individualColor / 255.0f);
	pIndividualColorsVBO->setData(vIndividualColors);
}