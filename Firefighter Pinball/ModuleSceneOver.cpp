#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleGui.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneOver.h"
#include "ModuleSceneStart.h"
#include "ModuleFonts.h"

ModuleSceneOver::ModuleSceneOver(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	OverScreen.x = 0;
	OverScreen.y = 0;
	OverScreen.w = 533;
	OverScreen.h = 799;

}

ModuleSceneOver::~ModuleSceneOver()
{}

// Load assets
bool ModuleSceneOver::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	TexOverScreen = App->textures->Load("assets/textures/scene_over.png");

	//Play stage music
	//App->audio->PlayMusic("assets/audio/music/stageTheme.ogg");
	App->scene_main->Disable();
	App->player->Disable();
	App->gui->Enable();
	App->fonts->Enable();

	DetermineHighScore();
	DisplayScoreText();
	DisplayPrevScoreText();
	DisplayHighScoreText();

	return ret;

}

void ModuleSceneOver::DetermineHighScore()
{
	if (App->scene_main->GetScore() > highScore)
		highScore = App->scene_main->GetScore();
}

void ModuleSceneOver::DisplayScoreText()
{
	p2SString score_text = (p2SString("%00000i", (App->scene_main->GetScore())));
	App->gui->CreateLabel({ 270,120 }, score_text, App->gui->GetFont(FONT), { 255, 255, 255, 255 }, this);
}

void ModuleSceneOver::DisplayPrevScoreText()
{
	App->gui->CreateLabel({ 0, 160 }, "Previous Score", App->gui->GetFont(FONT), { 255,255,0, 255 }, this);

	p2SString score_text = (p2SString("%00000i", (App->scene_main->GetPrevScore())));
	App->gui->CreateLabel({ 90, 200 }, score_text, App->gui->GetFont(FONT), { 255,255,0, 255 }, this);
}

void ModuleSceneOver::DisplayHighScoreText()
{
	App->gui->CreateLabel({ 360,160 }, "High Score", App->gui->GetFont(FONT), { 255,255,0, 255 }, this);

	p2SString score_text = (p2SString("%00000i", (highScore)));
	App->gui->CreateLabel({ 450,200 }, score_text, App->gui->GetFont(FONT), { 255,255,0, 255 }, this);
}

// Load assets
bool ModuleSceneOver::CleanUp()
{
	LOG("Unloading Intro Scene");
	App->textures->Unload(TexOverScreen);
	App->gui->Disable();
	App->fonts->Disable();
	

	return true;
}

// Update: draw background
update_status ModuleSceneOver::Update()
{
	// Draw everything --------------------------------------	
	App->renderer->Blit(TexOverScreen, 0, 0, &OverScreen);


	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade->FadeToBlack(this, App->scene_start, 1.5f);

	}



	return UPDATE_CONTINUE;
}