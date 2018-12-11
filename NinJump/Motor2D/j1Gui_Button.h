#ifndef __j1GUI_BUTTON_H__
#define __j1GUI_BUTTON_H__

#include "j1Gui_Object.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct _TTF_Font;
struct SDL_Color;



class j1Gui_Button : public j1Gui_Object
{
public:

	j1Gui_Button(Hover_Anim* anim, SDL_Texture* tex, SDL_Rect atlas_rect, iPoint pos, _TTF_Font* f, char* text, char* ID, Menu_Level menu_level);

	// Destructor
	virtual ~j1Gui_Button() {};


	void Blit();


private:
	_TTF_Font* font;
	char* text = nullptr;
	char* ID = nullptr;
	Animation* current_anim; 
	Hover_Anim* anim;

};


#endif // __j1GUI_BUTTON_H__
