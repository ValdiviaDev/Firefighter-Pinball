#ifndef __MODULESTARTSCREEN_H__
#define __MODULESTARTSCREEN_H__

#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

struct SDL_Texture;


class ModuleSceneOver : public Module
{
public:
	ModuleSceneOver(Application* app, bool start_enabled = false);
	~ModuleSceneOver();

	bool Start();

	void DetermineHighScore();
	void DisplayScoreText();
	void DisplayPrevScoreText();
	void DisplayHighScoreText();

	update_status Update();
	bool CleanUp();

public:

	SDL_Texture * TexOverScreen = nullptr;
	SDL_Rect OverScreen;

	uint prevScore = 0;
	uint highScore = 0;

};

#endif // __MODULESTARTSCREEN_H__