#ifndef __Application_H__
#define __Application_H__

#include "p2List.h"
#include "Globals.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class ModuleSceneIntro;
class ModulePhysics;
class ModuleFonts;
class ModuleGui;
class ModuleFadeToBlack;
class ModuleStartScreen;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* main_scene;
	ModulePhysics* physics;
	ModuleFonts* fonts;
	ModuleGui* gui;
	ModuleFadeToBlack* fade;
	ModuleStartScreen* start_scene;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};

extern Application* App;

#endif
