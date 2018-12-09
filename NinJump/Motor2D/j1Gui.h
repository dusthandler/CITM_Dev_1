#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"


#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

class j1Gui_Object; 
class j1Gui_Image; 
class j1Gui_Label; 
struct SDL_Texture; 
struct SDL_Rect; 
struct SDL_Color;
struct _TTF_Font; 


enum TYPE {
	Label,
	Button,
	Input_Text,
	Image,
	Checkbox
}; 

enum Hover_State {
	HOVER,
	CLICK,
	DRAG,
	OUTSIDE
};

enum Hierarchy {
	SCREEN = 1,
	PANEL = 2,
	LABEL = 3,
	IMAGE = 4,
	BUTTON = 5,
	BUTTON_LABEL = 6,
	BUTTON_IMAGE = 7
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	j1Gui_Image* Create_Image(SDL_Texture*, iPoint, SDL_Rect&, char* ID = nullptr);

	j1Gui_Label* Create_Label(iPoint, _TTF_Font*, char*, char* ID = nullptr);

	void Select_Clicked_Object(); 
	j1Gui_Object* Get_Clicked_Object(); 
	void Move_Clicked_Object(j1Gui_Object*);

	// void Delete_Object();

	virtual void Blit();

     const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<j1Gui_Object*> objects; 

	j1Gui_Image* UI_coin = nullptr; 
	j1Gui_Label* coin_score = nullptr; 

	j1Gui_Image* UI_lives = nullptr;
	j1Gui_Label* live_count = nullptr;

	// hovering and clicking

	uint hover_objects_queue = 0; 
	j1Gui_Object* clicked_object = nullptr; 
	bool move_object = false; 
public: 
	

public: 
	uint coins_collected = 0;                  // Where should this be?

};

#endif // __j1GUI_H__