#ifndef __j1MAIN_MENU_H__
#define __j1MAIN_MENU_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"

struct SDL_Texture;
struct _Mix_Music;

class j1Gui_Image;

class j1Main_Menu : public j1Module
{
public:

	j1Main_Menu();

	// Destructor
	virtual ~j1Main_Menu();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;

	
private:
	_Mix_Music* mus = nullptr;

	// ------- GUI objects --------

	j1Gui_Image* bg_image = nullptr; 
	SDL_Texture* bg_image_tex = nullptr; 
};

#endif // __j1MAIN_MENU_H__