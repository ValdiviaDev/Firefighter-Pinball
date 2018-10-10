#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "Module.h"
#include "UIElem.h"
#include "UIImage.h"
#include "UILabel.h"

enum FONT_TYPE {
	NO_FONT,
	FONT,
};


class ModuleGui : public Module
{
public:

	ModuleGui(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleGui();

	// Call before first frame
	bool Start();

	// Called before all Updates
	update_status PreUpdate();

	//Update
	update_status Update();

	// Called after all Updates
	update_status PostUpdate();

	// Called before quitting
	bool CleanUp();

	UIImage* CreateImage(iPoint pos, SDL_Rect image, const SDL_Texture* tex, Module* callback);
	UILabel* CreateLabel(iPoint pos, p2SString text, _TTF_Font* font, SDL_Color colour, Module* callback);
	
	_TTF_Font* GetFont(FONT_TYPE font_name);

	bool debug_print = false;

private:

	p2List<UIElem*> UIElements;
	_TTF_Font* scoreFont = nullptr;

};

#endif // __j1GUI_H__
