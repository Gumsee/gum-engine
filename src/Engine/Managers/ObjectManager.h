#pragma once
#include <Essentials/Settings.h>

#include "../Object/Object.h"
#include "../Object/Skeletal/AnimatedModel.h"
#include "../Object/Skybox.h"


#define OBJ_INSTANCED 	0
#define	OBJ_TEXTURED  	1
#define	OBJ_SKYBOX  	2
#define	OBJ_ANIMATED  	3

class Renderer3D;
class Camera;

class ObjectManager
{
private:
	SkyBox *pSkyBox;
	std::map<std::string, Object*> Objects;
	std::map<std::string, AnimatedModel*> AnimObjs;
	
public:
	ObjectManager(vec3 *sunDirection);
	~ObjectManager();

	static std::string MODEL_ASSETS_PATH;
	static int LAST_OBJECT_INSTANCE_ID;


	enum ExceptionTypes
	{
		WITHOUTREFLECTIVE,
		WITHOUTSHADOWS,
		WITHOUTSKYBOX,
	};


	void render(int exception = 0, ShaderProgram *shader = nullptr, bool noPrepare = false);
	void renderToGBuffer(ShaderProgram* gbufferShader);
    void renderExceptGBuffer(ShaderProgram* gbufferShader, Camera* camera);

	/**
     * Adds a Object to the Object map
	 *
	 *	ObjectPath: Name of the object (no complete path)
	 *	Shader:		The shader that corrensponds to the Object
	 *	Kind:		What kind of object should be created
	 *	Identifier:	The Identifier thats used in the program
	 */
	Object* addObject(Object* obj, std::string Identifier = "");

	void removeObject(Object *objToDelete);

    //Setter
	void setSkybox(SkyBox *skybox);

    //Getter
	Object* getObject(const std::string& name);
	Object* getObject(const unsigned int& vaoID);
	Instance* getInstanceByID(const unsigned int& id);
	bool hasObject(std::string name);
    std::map<std::string, Object*> getObjectsMap();
	AnimatedModel *getAnimObject(std::string name);
	SkyBox *getSkybox();

	unsigned int getObjectUnderMouse(Renderer3D* renderer) const;
	
	/*
	* This function gets called the Engine never call it yourself
	*/
	void clean();
};