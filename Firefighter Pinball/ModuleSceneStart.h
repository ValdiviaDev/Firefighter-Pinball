#ifndef __MODULESCENESTART_H__
#define __MODULESCENESTART_H__

#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

struct SDL_Texture;


class ModuleSceneStart : public Module
{
public:
	ModuleSceneStart(Application* app, bool start_enabled = true);
	~ModuleSceneStart();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture * TexIntroScreen = nullptr;
	SDL_Rect IntroScreen;
	

};

#endif // __MODULESCENESTART_H__