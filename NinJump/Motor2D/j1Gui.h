#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"  

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

class j1Gui_Object; 
class j1Gui_Image; 
class j1Gui_Label; 
class j1Gui_Button;
struct SDL_Texture; 
struct SDL_Rect; 
struct SDL_Color;
struct _TTF_Font; 

enum Menu_Level {
	Menu,
	Level
};
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


struct Hover_Anim {
	Animation a_Idle;
	Animation a_Hover;
	Animation a_Click;
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


	void Clean_Menu_GUI();
	void Clean_Level_GUI();

	// Called before quitting
	bool CleanUp();
	


	// TODO 2: Create the factory methods
	// Gui creation functions

	j1Gui_Image* Create_Image(SDL_Texture*, iPoint pos, SDL_Rect&, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level);

	j1Gui_Label* Create_Label(iPoint pos, _TTF_Font*, char*, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level);

	j1Gui_Button* Create_Button(Hover_Anim* anim, SDL_Texture* tex, SDL_Rect atlas_rect, iPoint pos, _TTF_Font* f, char* text, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level);

	

	void Select_Clicked_Object(); 
	j1Gui_Object* Get_Clicked_Object(); 
	void Move_Clicked_Object(j1Gui_Object*);

	

	virtual void Blit();
	void Generate_Menu_GUI(); 
	void Generate_Level_GUI();

     const SDL_Texture* GetAtlas() const;


public: 
	SDL_Texture* atlas;

private:
	p2SString atlas_file_name;
	p2List<j1Gui_Object*> objects; 

	// objects

	j1Gui_Image* UI_coin = nullptr; 
	j1Gui_Label* coin_score = nullptr; 

	j1Gui_Image* UI_lives = nullptr;
	j1Gui_Label* live_count = nullptr;

	j1Gui_Image* menu_image = nullptr;
	SDL_Texture* menu_image_tex = nullptr;

	j1Gui_Image* menu_label = nullptr;
	// hovering and clicking

	uint hover_objects_queue = 0; 
	j1Gui_Object* clicked_object = nullptr; 
	bool move_object = false; 

public: 
	bool create_menu_GUI = true; 
	bool create_level_GUI = false;


public: 
	uint coins_collected = 0;                  // Where should this be?

};

#endif // __j1GUI_H__