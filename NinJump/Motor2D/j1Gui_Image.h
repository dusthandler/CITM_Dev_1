#ifndef __j1GUI_IMAGE_H__
#define __j1GUI_IMAGE_H__

#include "j1Gui_Object.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct _TTF_Font;

class j1Gui_Image : public j1Gui_Object
{
public:

	j1Gui_Image(SDL_Texture* tex, iPoint pos, SDL_Rect atlas_rect, char* ID, Menu_Level menu_level, j1Gui_Object* parent);


	// Destructor
	virtual ~j1Gui_Image() {};
	void Blit();
	

private:

	char* ID = nullptr; 
	
};


#endif // __j1GUI_IMAGE_H__
