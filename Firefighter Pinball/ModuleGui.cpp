#include "Globals.h"    
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleGui.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleGui::~ModuleGui()
{}



// Called before the first frame
bool ModuleGui::Start()
{
	//Mega_Man_2 = App->font->Load(Mega_Man_2_font_path.GetString(), 10);
	scoreFont = App->fonts->Load("assets/Fonts/gomarice_game_continue_02.ttf", 40);
	p2List_item<UIElem*>* elem = UIElements.getFirst();
	while (elem != nullptr) {
		elem = nullptr;
		//elem = elem->next;
	}

	return true;
}

// Update all guis
update_status ModuleGui::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update()
{
	int a = 3;
	p2List_item<UIElem*>* elem = UIElements.getFirst();
	while (elem != nullptr) {
		elem->data->Update();
		elem = elem->next;
	}
	scoreFont;
	return UPDATE_CONTINUE;
}

// Called after all Updates
update_status ModuleGui::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UIElem*>* elem = UIElements.getFirst();
	while (elem != nullptr) {
		delete elem->data;
		elem->data = nullptr;
		elem = elem->next;
	}

	return true;
}


UIImage* ModuleGui::CreateImage(iPoint pos, SDL_Rect image, const SDL_Texture * tex, Module * callback)
{
	UIImage* ret = new UIImage(pos, image, tex, callback);
	UIElements.add(ret);
	return ret;
}

UILabel* ModuleGui::CreateLabel(iPoint pos, p2SString text, _TTF_Font* font, SDL_Color colour, Module* callback) {
	UILabel* ret = new UILabel(pos, text, font, colour, callback);
	UIElements.add(ret);
	return ret;
}


_TTF_Font* ModuleGui::GetFont(FONT_TYPE font_name) {
	return scoreFont;
}

// class Gui ---------------------------------------------------
