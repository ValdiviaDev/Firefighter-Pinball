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
class ModuleSceneMain;
class ModulePhysics;
class ModuleFonts;
class ModuleGui;
class ModuleFadeToBlack;
class ModuleSceneStart;
class ModuleSceneOver;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneMain* scene_main;
	ModulePhysics* physics;
	ModuleFonts* fonts;
	ModuleGui* gui;
	ModuleFadeToBlack* fade;
	ModuleSceneStart* scene_start;
	ModuleSceneOver* scene_over;

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
