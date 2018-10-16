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


	return ret;

}

// Load assets
bool ModuleSceneOver::CleanUp()
{
	LOG("Unloading Intro Scene");
	App->textures->Unload(TexOverScreen);
	App->scene_start->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneOver::Update()
{
	// Draw everything --------------------------------------	
	App->renderer->Blit(TexOverScreen, 0, 0, &OverScreen);


	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		App->fade->FadeToBlack(this, App->scene_start, 1.5f);

	}



	return UPDATE_CONTINUE;
}