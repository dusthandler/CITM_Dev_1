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

	j1Gui_Button(/*Hover_Anim* hover_rects,*/ SDL_Texture* tex, iPoint pos, char* ID, Menu_Level menu_level, j1Gui_Object* parent);

	// Destructor
	virtual ~j1Gui_Button() {};


	void Blit();


private:
	_TTF_Font* font;
	char* text = nullptr;
	char* ID = nullptr;
	

};


#endif // __j1GUI_BUTTON_H__
