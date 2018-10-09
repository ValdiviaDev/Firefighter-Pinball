#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"
#include "p2List.h"

//#define DEFAULT_FONT "fonts/Mega_Man_2.ttf"
//#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class ModuleFonts : public Module
{
public:

	ModuleFonts(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleFonts();

	// Called before render is available
	//bool Awake(pugi::xml_node&);
	bool Init();

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	p2List<_TTF_Font*>	fonts;
	_TTF_Font*			default;
};


#endif // __ModuleFonts_H__
