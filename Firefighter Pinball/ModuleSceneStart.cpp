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
#include "ModuleSceneStart.h"

ModuleSceneStart::ModuleSceneStart(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	IntroScreen.x = 0;
	IntroScreen.y = 0;
	IntroScreen.w = 533;
	IntroScreen.h = 799;

}

ModuleSceneStart::~ModuleSceneStart()
{}

// Load assets
bool ModuleSceneStart::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	TexIntroScreen = App->textures->Load("assets/textures/Start_Screen.png");
	
	//Play intro song music
	App->audio->PlayMusic("assets/audio/music/introTheme.ogg");

	App->scene_main->Disable();
	App->player->Disable();
	
	
	return ret;

}

// Load assets
bool ModuleSceneStart::CleanUp()
{
	LOG("Unloading Intro Scene");
	App->textures->Unload(TexIntroScreen);
	App->scene_start->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneStart::Update()
{
	// Draw everything --------------------------------------	
	App->renderer->Blit(TexIntroScreen, 0, 0, &IntroScreen);


	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade->FadeToBlack(this, App->scene_main, 1.5f);
			
	}



	return UPDATE_CONTINUE;
}