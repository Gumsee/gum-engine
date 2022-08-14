// #pragma once

// #include <thread>

// #include "Essentials/Window.h"
// #include "Essentials/FPS.h"
// #include "Essentials/Camera.h"
// #include "Essentials/Input/Input.h"
// #include "Essentials/Cursor.h"
// #include "Essentials/Renderer3D.h"

// #include "Extras/Debug.h"


// #include "Physics/Physics.h"

// #include "PostProcessing/ShadowMapping/ShadowMapping.h"


// #include "GUI/GUIManager.h"
// #include "GUI/Font/FontManager.h"
// #include "Texture/TextureManager.h"
// #include "Material/MaterialManager.h"
// #include "Shader/ShaderManager.h"
// #include "Audio/AudioManager.h"

// namespace GumEngine
// {
// 	enum UpdateTypes
// 	{
// 		EVENTS,
// 		CAMERA,
// 		MOUSE,
// 		FPSUPDATE,
// 		SHADER,
// 		SKY,
// 		PARTICLE,
// 		PHYSICS,
// 		UPDATEABLE,
// 		LOADABLE,
// 		LIGHTS,
// 		CONSOLE,
// 		GUI,
// 		AUDIO,
// 		ALL,
// 	};

// 	extern ShaderManager *Shaders;
// 	extern TextureManager *Textures;
// 	extern GUIManager *GUIS;
// 	extern FontManager *Fonts;
// 	extern MaterialManager *Materials;
// 	extern AudioManager *Audios;
// 	extern GumPhysics *Physics;
// 	extern Cursor *cursor;

// 	extern unsigned int iLastObjectID;
// 	extern std::string shellStr;
	
//     extern Renderer3D *DefaultRenderer;
//     extern OutlineRenderer* DefaultOutlineRenderer;
// 	extern Framebuffer* CurrentFramebuffer;
// 	extern Camera* ActiveCamera;

// 	extern void InitGLVar();
// 	extern void demoLoop();
// 	extern bool checkIsRunning();
// 	extern bool checkIfStopped();
// 	extern void updateEvents();
// 	extern void stop();
// 	extern void init(const std::string assetPath, bool Fullscreen, std::string title, const ivec2 WindowSize = ivec2(), bool inPercent = false);
// 	extern void initEssentials();
// 	extern void initAssets(const std::string assetPath);
// 	extern void initContextWindow(std::string title, const ivec2 WindowSize = ivec2(), bool inPercent = false);
// 	extern void initRenderer(Box* rendercanvas = nullptr);

	

// 	extern void prepareRender();

// 	extern void render();
// 	extern void renderGUI();

// 	extern void finishRender();

// 	/*Updates selected Section
// 	*
// 	* Type CLOSE to check if the window was closed
// 	* Type Mouse to update mouse variables
// 	* Type CAMERA to update the camera
// 	* Type FPS (every frame) to update the current Frametime
// 	* Type SHADER to update all shaders that have been added
// 	* Type EVENTS to update all window events
// 	* Type ALL to update all above
// 	*/
// 	extern bool update(const unsigned int Type);

// 	extern void cleanup();
// };
