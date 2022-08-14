#include "Engine.h"
#include <iostream>
#include "Essentials/OpenGL/HardwareInfo.h"
#include "Essentials/Input/Controls.h"
#include "Essentials/Input/Keyboard.h"
#include "Essentials/Input/Mouse.h"
#include "Essentials/Settings.h"
#include "Essentials/Window.h"
#include "Globals.h"

namespace GumEngine 
{
	ShaderManager *Shaders;
	TextureManager *Textures;
	GUIManager *GUIS;
	FontManager *Fonts;
	MaterialManager *Materials;
	AudioManager *Audios;
	GumPhysics *Physics;
	Cursor *cursor;

	std::string shellStr = "> ";
	unsigned int iLastObjectID = 0;
    Renderer3D *DefaultRenderer;
    OutlineRenderer* DefaultOutlineRenderer;
	Framebuffer* CurrentFramebuffer = nullptr;

	void initEssentials()
	{
		Settings::init();
		FPS::init();
		Input::Mouse = new Input::InputMouseClass();
		Input::Keyboard = new Input::InputKeyboardClass();
		Input::Controls = new Input::InputControlsClass();
		InitGLVar();
		Gum::HardwareInfo::init();
		ActiveCamera = new Camera(Gum::Window->getSize(), nullptr);
	}
	
	void initAssets(const std::string assetPath)
	{
        GumGlobals::ASSETS_PATH = assetPath;
        GumGlobals::MODEL_ASSETS_PATH = assetPath + "/models/";
        GumGlobals::TEXTURE_ASSETS_PATH = assetPath + "/textures/";
        GumGlobals::SHADER_ASSETS_PATH = assetPath + "/shaders/";
        GumGlobals::FONT_ASSETS_PATH = assetPath + "/fonts/";
        GumGlobals::AUDIO_ASSETS_PATH = assetPath + "/audios/";
        GumGlobals::MATERIAL_ASSETS_PATH = assetPath + "/materials/";
        GumGlobals::GUI_ASSETS_PATH = assetPath + "/guis/";

		Gum::Output::log("Initializing Managers...");
		Textures = new TextureManager();
		Materials = new MaterialManager();
		Shaders = new ShaderManager();
        Shaders->initDefaultShader();
        Shaders->initStencilShader();
		Physics = new GumPhysics();				//Depends on Shaders
		Fonts = new FontManager();
		GUIS = new GUIManager();  					//Depends on Shaders and Fonts
		Audios = new AudioManager();
		Gum::Output::log("Initialized Managers!");
	}

	void initContextWindow(std::string title, const ivec2 WindowSize, bool inPercent)
	{
		Gum::Window = new Gum::GumWindowClass(false, title, WindowSize, inPercent);

		//Console
		//Gum::Output::log("Initializing Console...");
		//con::create("/home/gumse/Program/cpp/3DGame/Fonts/SourceCodeVariable-Roman.otf", vec2(Gum::Window->getSize().x  * 0.5, Gum::Window->getSize().y  * 0.5), 15, input_function);
		//con::init();
		//ConsoleCommands::initCommands(this);
		//std::thread ConsoleThread(ConsoleUpdate);
		//ConsoleThread.detach();
	}

	void initRenderer(Box* rendercanvas)
	{
		if(rendercanvas == nullptr)
			rendercanvas = new Box(ivec2(0,0), Gum::Window->getRenderQuadSize(), 0);

    	DefaultOutlineRenderer = new OutlineRenderer(rendercanvas);
        DefaultRenderer = new Renderer3D(rendercanvas);

		cursor = new Cursor();
		cursor->create();
	}

	void init(const std::string assetPath, bool Fullscreen, std::string title, const ivec2 WindowSize, bool inPercent)
	{
		std::cout << "Initializing Engine..." << std::endl;

		Gum::Output::init("./Engine.log", "./EngineDebug.log");
		Gum::Output::log("Initializing GumEngine...");

		initContextWindow(title, WindowSize, inPercent);
		initEssentials();
		initAssets(assetPath);
		initRenderer();

		Gum::Output::log("Finished initialization!");
		std::cout << "Finished initialization!" << std::endl;
	}

	void prepareRender()
	{
		if(Gum::Output::stoprunning)
		{
			stop();
		}
		glClearDepth(1.0);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void render()
	{
		DefaultRenderer->render();

		//Debug Drawing..
		Physics->drawDebug();
	}

	void renderGUI()
	{
		Gum::Output::debug("Rendering GUI");
		GUIS->render();
		cursor->render();
	}

	void finishRender()
	{
		Gum::Window->finishRender();
	}

	bool update(const unsigned int Type)
	{
		if(Gum::Output::stoprunning)
		{
			stop();
			return false;
		}
		
		cursor->update();
		if (Type == MOUSE)
		{
			Gum::Output::debug("Updating Mouse");
			Input::Mouse->update();

			//Mouse::mouseOnID = pickedID;
			//TODO
			//Gum::Window->window->setTitle("Engine   FPS: " + std::to_string((int)(1 / FPS::get())));
		}
		else if (Type == CAMERA) 	{ ActiveCamera->update(); }
		else if (Type == AUDIO) 	{ Audios->update(); }
		else if (Type == FPSUPDATE) { FPS::update(); }
		else if (Type == SHADER) 	{ Shaders->update(); }
		else if (Type == GUI) 		{ GUIS->update(); }
		else if (Type == EVENTS) 	{ updateEvents(); }
		else if (Type == PARTICLE) 	{ }
		else if (Type == PHYSICS) 	{ Physics->update(); }
		else if (Type == UPDATEABLE)
		{
			Gum::Output::debug("Updating Updateable");
			DefaultRenderer->update();
			Textures->update();
		}
		else if(Type == CONSOLE)
		{
			//Gum::Window->window->setActive(false);
			//con::window->setActive(true);
			//con::update();
			//con::window->setActive(false);
			//Gum::Window->window->setActive(true);
		}
		else if (Type == ALL)
		{
			Gum::Output::debug("Updating ALL");
			update(MOUSE);
			update(FPSUPDATE);
			update(PHYSICS);
			update(EVENTS);
			update(LIGHTS);
			update(CAMERA);
			update(SHADER);
			update(AUDIO);
			update(UPDATEABLE);
			update(GUI);
			update(PARTICLE);
			update(CONSOLE);
			update(SKY);
		}
		return true;
	}

	void updateEvents()
	{
		Gum::Output::debug("Updating Events");
		Input::Controls->setCurrentKey(0);
		Input::Keyboard->reset();

		sf::Event event;
		while(Gum::Window->pollEvent(event))
		{
			Input::Mouse->handleEvents(event);
			Input::Keyboard->handleEvents(event);
			Gum::Window->handleEvents(event);
			
			switch (event.type)
			{
				case sf::Event::MouseMoved:
					Input::Mouse->setInstanceIDUnderMouse(DefaultRenderer->getWorld()->getObjectManager()->getObjectUnderMouse(DefaultRenderer));
					break;
					
				case sf::Event::Resized:
					GUIS->windowResize();
					DefaultRenderer->setResolution(Gum::Window->getSize());
    				ActiveCamera->updateProjection(Gum::Window->getSize()); 
					cursor->updateProjection();
					break;
				case sf::Event::JoystickButtonPressed:
				case sf::Event::JoystickButtonReleased:
				case sf::Event::JoystickMoved:
				case sf::Event::JoystickConnected:
				case sf::Event::JoystickDisconnected:
				case sf::Event::TouchBegan:
				case sf::Event::TouchMoved:
				case sf::Event::TouchEnded:
				default:
					break;
			}
		}

		if(Input::Keyboard->checkKeySinglepress("Numpad5"))
		{
			if(ActiveCamera->getProjectionMode() == Camera::ProjectionModes::PERSPECTIVE)
				ActiveCamera->setProjectionMode(Camera::ProjectionModes::ORTHOGRAPHIC);
			else
				ActiveCamera->setProjectionMode(Camera::ProjectionModes::PERSPECTIVE);
		}
	}

	void demoLoop()
	{
		while(checkIsRunning())
		{
			update(ALL);
			prepareRender();
			render();
			renderGUI();
			finishRender();
		}
	}

	void InitGLVar()
	{
		//Initialize OpenGL Variables and glew

		Gum::Output::info("Initializing OpenGL Variables...");

		GLenum err = glewInit();

		if(err != GLEW_OK)
			Gum::Output::error("GLEW ERROR: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glDepthFunc(GL_LEQUAL);
		//glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		Gum::Output::info("Successfully initialized OpenGL Variables!");
	}

	bool checkIsRunning() { return Gum::Window->isOpen(); }

	void stop()
	{
		//con::stop();
		Gum::Window->close();
	}

	bool checkIfStopped()
	{
		if(!Gum::Window->isOpen())
		{
			cleanup();
			Gum::Output::info("GumEngine Stopped!");
		}
		return !Gum::Window->isOpen();
	}

	void cleanup()
	{
		Textures->clean();
		Shaders->clean();
		GUIS->clean();
		Materials->clean();

		delete Textures;
		delete Shaders;
		delete GUIS;
		delete Materials;

		Textures = nullptr;
		Shaders = nullptr;
		GUIS = nullptr;
		Materials = nullptr;
	}

}