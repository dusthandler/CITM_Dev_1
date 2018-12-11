#ifndef __j1GUI_OBJECT_H__
#define __j1GUI_OBJECT_H__

#include "j1Gui.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

class Animation;

class j1Gui_Object : public j1Gui
{
public:

	j1Gui_Object(iPoint pos) {
		this->pos = pos; 
	
	};

	// Destructor
	virtual ~j1Gui_Object() {};

	// Called when before render is available
	virtual bool Awake(pugi::xml_node&) {
		return true; 
	};

	
public:

	iPoint pos; 
	SDL_Rect rect; 
	TYPE type; 
	Animation* anim; 
	SDL_Texture* tex; 
	bool selected = false; 
	Hierarchy hierarchy; 
	Hover_State hover_state = Hover_State::OUTSIDE; 
	Menu_Level menu_level; 
};


#endif // __j1GUI_OBJECT_H__
