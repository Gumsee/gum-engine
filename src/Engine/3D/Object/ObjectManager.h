#pragma once
#include <Essentials/Settings.h>
#include <functional>

#include "../Object/Skeletal/AnimatedModel.h"
#include "../Object/Skybox.h"


#define OBJ_INSTANCED 	0
#define	OBJ_TEXTURED  	1
#define	OBJ_SKYBOX  	2
#define	OBJ_ANIMATED  	3

class Renderer3D;
class Camera;

typedef std::function<void(Object3D*)> AddObjectCallback;

class ObjectManager
{
private:
	SkyBox *pSkyBox;
	std::vector<Object3D*> vObjects;
    AddObjectCallback pAddObjectCallback;
	
public:
	ObjectManager(vec3 *sunDirection);
	~ObjectManager();

	static std::string MODEL_ASSETS_PATH;


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
	Object3D* addObject(Object3D* obj, std::string Identifier = "");

	void removeObject(Object3D *objToDelete);

    //Setter
	void setSkybox(SkyBox *skybox);
    void onAddObject(AddObjectCallback callback);

    //Getter
	Object3D* getObject(const std::string& name);
	Object3D* getObject(const unsigned int& vaoID);
	Object3DInstance* getInstanceByID(const unsigned int& id);
	bool hasObject(std::string name);
	SkyBox *getSkybox();
	unsigned int numObjects();

	unsigned int getObjectUnderMouse(Renderer3D* renderer) const;
};