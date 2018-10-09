#ifndef __LABEL_H__
#define __LABEL_H__

#include "p2SString.h"
#include "UIElem.h"
struct _TTF_Font;

class UILabel : public UIElem {

private:

	p2SString text;
	_TTF_Font* font = nullptr;
	SDL_Color colour;

public:

	UILabel(iPoint screen_area, p2SString text, _TTF_Font* font, SDL_Color colour, Module* callback);
	void Draw();
	void ChangeText(p2SString text);
	p2SString GetText() const;
	void ChangeColor(SDL_Color colour);

};


#endif
