#pragma once
#include <Essentials/Settings.h>
#include <functional>
#include <unordered_map>

#include "../Object/Skeletal/AnimatedModel.h"
#include "../Object/Skybox.h"
#include "../../Lightning/G_Buffer.h"
#include "../Lightning/Lightning.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/ShaderProgram.h"


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
	std::unordered_map<ShaderProgram*, std::vector<Object3D*> > mObjectsForward;
	std::unordered_map<ShaderProgram*, std::vector<Object3D*> > mObjectsDefered;
    AddObjectCallback pAddObjectCallback;
	
public:
	ObjectManager(vec3 *sunDirection);
	~ObjectManager();

	static Gum::File MODEL_ASSETS_PATH;


	enum ExceptionTypes
	{
		WITHOUTREFLECTIVE,
		WITHOUTSHADOWS,
		WITHOUTSKYBOX,
	};


	void renderSky();
	void renderDefered(G_Buffer* gbuffer, Box* rendercanvas);
	void renderForward();
	void renderEverything();
	void renderEverythingMeshesOnly();
    void renderIDs();

	/**
     * Adds a Object to the Object map
	 *
	 *	ObjectPath: Name of the object (no complete path)
	 *	Shader:		The shader that corrensponds to the Object
	 *	Kind:		What kind of object should be created
	 *	Identifier:	The Identifier thats used in the program
	 */
	Object3D* addObject(Object3D* obj, ShaderProgram* shader = Lightning::getDefaultShaderProgram(), bool defered = true);
	Object3D* addObjectForwardRendered(Object3D* obj, ShaderProgram* shader);

	void removeObject(Object3D *objToDelete);
    void updateShaderPrograms(Camera* camera);

    //Setter
	void setSkybox(SkyBox *skybox);
    void onAddObject(AddObjectCallback callback);

    //Getter
	Object3D* getObject(const std::string& name);
    void iterateThroughObjects(std::function<void(Object3D* obj, ShaderProgram* shader, bool defered)> callback);
	Object3DInstance* getInstanceByID(const unsigned int& id);
	bool hasObject(std::string name);
	SkyBox *getSkybox();
	unsigned int numObjects();

	unsigned int getObjectUnderMouse(Renderer3D* renderer) const;
};