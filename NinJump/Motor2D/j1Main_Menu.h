#ifndef __j1MAIN_MENU_H__
#define __j1MAIN_MENU_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "p2Point.h"
#include "j1Timer.h"

enum Active_Menu {
	MAIN,
    SETTINGS,
	CREDITS,
	NONE
};

struct SDL_Texture;
struct _Mix_Music;

class j1Gui_Image;
class j1Gui_Object;

class j1Main_Menu : public j1Module
{
public:

	j1Main_Menu();

	// Destructor
	virtual ~j1Main_Menu();



	// Called before the first frame
	bool Start();
	bool Update(float dt);
	bool PostUpdate();

	
	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;
	

public: 
	Active_Menu active_menu = Active_Menu::MAIN;
	
private:
	// main
	_Mix_Music* mus = nullptr;
	// settings
	_Mix_Music* mus_cred = nullptr;
	// credits
	_Mix_Music* mus_set = nullptr;




	
};

void Clicked_Pause();
void Return_to_Main();
void Clicked_Credits();
void Clicked_Settings();
void Clicked_Exit();
void Clicked_Play();
void Clicked_Continue();//peta al salir???
void Move_Object(j1Gui_Object*);
void Move_Slider(j1Gui_Object*,iPoint);
#endif // __j1MAIN_MENU_H__