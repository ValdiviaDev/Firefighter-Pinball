#include "Application.h"
#include "UIImage.h"

UIImage::UIImage(iPoint screen_area, SDL_Rect image, const SDL_Texture* tex, Module* callback)
{
	type = IMAGE;
	this->screen_area = screen_area;
	this->tex = tex;
	this->callback = callback;
	UI_logic_rect = image;
}

void UIImage::Draw() {

	App->renderer->Blit((SDL_Texture*)tex, screen_area.x, screen_area.y, &UI_logic_rect, 0.0f);

}
void UIImage::ChangeImage(SDL_Rect new_image) {

	UI_logic_rect = new_image;

}