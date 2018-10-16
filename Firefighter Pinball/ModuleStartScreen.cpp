#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleGui.h"
#include "ModuleFadeToBlack.h"
#include "ModuleStartScreen.h"

ModuleStartScreen::ModuleStartScreen(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	IntroScreen.x = 0;
	IntroScreen.y = 0;
	IntroScreen.w = 533;
	IntroScreen.h = 799;

}

ModuleStartScreen::~ModuleStartScreen()
{}

// Load assets
bool ModuleStartScreen::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	TexIntroScreen = App->textures->Load("assets/textures/Start_Screen.png");
	
	//App->gui->Disable();

	
	
	return ret;

}

// Load assets
bool ModuleStartScreen::CleanUp()
{
	LOG("Unloading Intro Scene");
	App->textures->Unload(TexIntroScreen);


	return true;
}

// Update: draw background
update_status ModuleStartScreen::Update()
{
	// Draw everything --------------------------------------	
	App->renderer->Blit(TexIntroScreen, 0, 0, &IntroScreen);


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->fade->FadeToBlack(this, App->scene_intro, 1.5f);
			
	}



	return UPDATE_CONTINUE;
}