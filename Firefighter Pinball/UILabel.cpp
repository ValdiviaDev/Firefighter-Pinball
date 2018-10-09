#include "Application.h"
#include "UILabel.h"
#include "ModuleFonts.h"
#include "ModuleTextures.h"


UILabel::UILabel(iPoint screen_area, p2SString text, _TTF_Font* font, SDL_Color colour, Module* callback) : text(text), font(font), colour(colour)
{
	type = LABEL;
	this->screen_area = screen_area;
	this->callback = callback;

	this->text = text;
	tex = App->fonts->Print(text.GetString(), colour, font);
	UI_logic_rect.x = 0;
	UI_logic_rect.y = 0;
	SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &UI_logic_rect.w, &UI_logic_rect.h);

}

void UILabel::Draw() {
	App->renderer->Blit((SDL_Texture*)tex, screen_area.x, screen_area.y, &UI_logic_rect, 0.0f);
}

void UILabel::ChangeText(p2SString text) {

	this->text = text;
	App->textures->Unload((SDL_Texture*)tex);
	tex = App->fonts->Print(text.GetString(), colour, font);

}

p2SString UILabel::GetText() const {
	return text;
}

void UILabel::ChangeColor(SDL_Color colour) {

	App->textures->Unload((SDL_Texture*)tex);
	tex = App->fonts->Print(text.GetString(), colour, font);

}
