#ifndef __j1GUI_LABEL_H__
#define __j1GUI_LABEL_H__

#include "j1Gui_Object.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct _TTF_Font;
struct SDL_Color; 

class j1Gui_Label : public j1Gui_Object
{
public:

	j1Gui_Label(iPoint pos, _TTF_Font* f, char* text, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable);

	// Destructor
	virtual ~j1Gui_Label() {};

	void Init_Text(); 
	void Blit(); 
	

private:
	_TTF_Font* font; 
	bool has_button = false; 

};


#endif // __j1GUI_LABEL_H__