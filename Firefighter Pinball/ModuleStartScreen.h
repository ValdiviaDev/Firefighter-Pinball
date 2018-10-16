#ifndef __MODULELOSEIMAGE_H__
#define __MODULELOSEIMAGE_H__

#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

struct SDL_Texture;


class ModuleStartScreen : public Module
{
public:
	ModuleStartScreen(Application* app, bool start_enabled = true);
	~ModuleStartScreen();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture * TexIntroScreen = nullptr;
	SDL_Rect IntroScreen;
	

};

#endif // __MODULESTARTINGIMAGE_H__