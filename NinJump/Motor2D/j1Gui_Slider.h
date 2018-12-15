#ifndef __j1GUI_SLIDER_H__
#define __j1GUI_SLIDER_H__

#include "j1Gui_Object.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct _TTF_Font;


class j1Gui_Slider: public j1Gui_Object
{
public:

	j1Gui_Slider(SDL_Texture* tex, iPoint pos, SDL_Rect rect, j1Gui_Object* parent, char* ID, Menu_Level menu_level, bool draggable);


	// Destructor
	virtual ~j1Gui_Slider() {};
	void Blit();
	

private:


};


#endif // __j1GUI_SLIDER_H__

