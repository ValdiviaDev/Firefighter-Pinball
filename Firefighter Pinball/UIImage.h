#ifndef __UIImage_H__
#define __UIImage_H__

#include "UIElem.h"

class UIImage : public UIElem {

private:

public:
	UIImage(iPoint screen_area, SDL_Rect image, const SDL_Texture* tex, Module* callback);
	void Draw();
	void ChangeImage(SDL_Texture* newTex);
};



#endif
