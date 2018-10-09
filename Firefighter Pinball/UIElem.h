#ifndef __UIElem_H__
#define __UIElem_H__

#include "ModuleRender.h"
#include "p2Point.h"

enum UI_TYPE {
	TYPE_NONE,
	LABEL,
};


class UIElem {

protected:

	iPoint screen_area = { 0,0 };
	SDL_Rect UI_logic_rect;
	const SDL_Texture* tex = nullptr;
	UI_TYPE type = TYPE_NONE;
	Module* callback = nullptr;

public:
	UIElem();
	virtual ~UIElem();
	void Update();
	virtual void Draw();
	UI_TYPE GetType();

};


#endif
