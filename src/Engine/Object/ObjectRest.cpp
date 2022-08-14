#include "Object.h"
#include <random>
#include <fstream>
#include <Essentials/Tools.h>

#include "../Managers/ObjectManager.h"

//Side Functions
void Object::writeObjectInfoToFile(std::string filename)  //Move to fileparser
{
	Gum::Output::info("Writing Object Info " + getName() + " to " + filename);
	std::ofstream file(filename);

	//General Info
	file << "//General Information\n";
	file << "std::string Name = " << this->getName() << ";\n";
	file << "bool hasBackface = " << (this->pProperties->hasbackface ? "true" : "false") << "; \n";
	file << "bool hasInverseCulling = " << (this->pProperties->inverseCulling ? "true" : "false") << "; \n";
	file << "int ObjectType = " << std::to_string(this->pProperties->ObjectType) << ";\n";

	//Vertex Info

	std::string verticesString;
	for(size_t i = 0; i < pProperties->pMesh->numVertices(); i++) { verticesString += pProperties->pMesh->getVertex(i).toString("vec3(", "vec2(") + ", \n"; };

	std::string indicesString;
	for(size_t i = 0; i < pProperties->pMesh->numIndices(); i++) { indicesString += std::to_string(pProperties->pMesh->getIndex(i)) + ", "; };
	
	file << "\n//Vertices Information\n";
	file << "std::vector<Vertex> vertices = { \n" << verticesString << "};\n";
	file << "std::vector<int> indices = {" << indicesString << "};\n";

	/*file << "\n#Transformation Information\n";
	file << "Position: " << Tools::Vec3ToString(this->pProperties->getPosition()) << "\n";
	file << "Rotation: " << Tools::Vec3ToString(this->pProperties->getRotationEuler()) << "\n";
	file << "Scale: " << Tools::Vec3ToString(this->pProperties->getScale()) << "\n";*/

	file.close();
}



void Object::clean()
{
	//for(size_t i = 0; i < vInstances.size(); i++)
	{
	//	GumEngine::Physics->getWorld()->removeRigidBody(vInstances[i]->getBody());
	}
}


void Object::addInstance() { addInstance(new Instance(*vInstances[0])); }

void Object::addInstance(Instance *inst)
{
	inst->setBody(nullptr);
	inst->parentObject = this;
	inst->id = ObjectManager::LAST_OBJECT_INSTANCE_ID++;
	vInstances.push_back(inst);
	vTransforms.push_back(inst->getMatrix());
	pTransMatricesVBO->setData(vTransforms, GL_DYNAMIC_DRAW);
	/*for(size_t i = 0; i < vInstances.size(); i++)
	{
		vInstances[i]->setMatrixPtr(pTransMatricesVBO->getDataPointer(i, 1));
	}*/

	//Get individual ID
	createIndividualColor(inst);
	if(vInstances.size() > 1)
		pProperties->isInstanced = true;
}

void Object::operator++() { addInstance(); }

//Setter
void Object::RenderToShadowMap(bool var) 					{ this->pProperties->RendererdToShadowMap = var;	}
void Object::setMaterial(Material *mat) 					{ this->pMaterial = mat; }
void Object::setProperties(ObjectProperties *properties) 	{ this->pProperties = properties; }
void Object::setInstance(Instance *instance, int index) 	{ this->vInstances[index] = instance; }
void Object::setShader(ShaderProgram *shader) 				{ this->pShader = shader; }
void Object::setName(std::string name) 						{ this->pProperties->Name = name; }
void Object::setTessellatedShader(bool val) 				{ this->pProperties->renderTessellated = val; }
void Object::setTranformationMatrix(mat4 matrix, int index) { this->vInstances[index]->setMatrix(matrix); }
void Object::setOutline(bool DrawOutline) 					{ this->pProperties->outline = DrawOutline; }
void Object::setOutlineColor(vec3 outlineColor) 			{ this->pProperties->outlineColor = outlineColor; }
void Object::setOutlineThickness(float thickness) 			{ this->pProperties->outlineThickness = thickness; }

//Throughput
void Object::setPosition(vec3 pos, int index)				{ this->vInstances[index]->setPosition(pos); }
void Object::setRotation(quat rot, int index)				{ this->vInstances[index]->setRotation(rot); }
void Object::setRotation(vec3 rot, int index)				{ this->vInstances[index]->setRotation(rot); }
void Object::setScale(vec3 scale, int index)				{ this->vInstances[index]->setScale(scale); }
void Object::setScale(float scale, int index)				{ this->vInstances[index]->setScale(scale); }
void Object::increasePosition(vec3 pos, int index)			{ this->vInstances[index]->increasePosition(pos); }
void Object::increaseRotation(quat rot, int index)			{ this->vInstances[index]->increaseRotation(rot); }
void Object::increaseRotation(vec3 rot, int index)			{ this->vInstances[index]->increaseRotation(rot); }
void Object::increaseScale(vec3 scale, int index)			{ this->vInstances[index]->increaseScale(scale); }


//Getter
btRigidBody* 		Object::getRigidBody(int index) 						{ return vInstances[index]->getBody(); }
ShaderProgram*		Object::getShader() 									{ return this->pShader; }
mat4*		    	Object::getTransformationMatrix(int index) 				{ return vInstances[index]->getMatrixPtr(); }
std::string 		Object::getName() 										{ return pProperties->Name; }
ObjectProperties*	Object::getProperties() 								{ return pProperties; }
Instance* 			Object::getInstance(int index)							{ return vInstances[index]; }
GLuint 				Object::getShaderID() 									{ return pShader->getProgramID(); }
Material* 			Object::getMaterial() 									{ return pMaterial; }
int 				Object::numInstances() 									{ return vInstances.size(); }
bool 				Object::isMouseOn(int index) 							{ return vInstances[index]->ismouseon; }
bool 				Object::isRendererdToShadowMap() 						{ return pProperties->RendererdToShadowMap; }
VertexArrayObject*  Object::getVertexArrayObject()                          { return pVertexArrayObject; }

//Throughput
vec3 			Object::getPosition(int index)							{ return this->vInstances[index]->getPosition(); }
quat 			Object::getRotation(int index)							{ return this->vInstances[index]->getRotation(); }
vec3 			Object::getScale(int index)								{ return this->vInstances[index]->getScale(); }