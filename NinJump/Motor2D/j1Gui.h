#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Main_Menu.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"  

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

class j1Gui_Object; 
class j1Gui_Image; 
class j1Gui_Label; 
class j1Gui_Button;
class j1Gui_Slider;
struct SDL_Texture; 
struct SDL_Rect; 
struct SDL_Color;
struct _TTF_Font; 

enum Menu_Level {
	Main_Menu,
	Settings_Menu,
	Credits_Menu,
	Level
};

enum Next_Menu {
	MAIN_NEXT,
	SETTINGS_NEXT,
	CREDITS_NEXT,
	NONE_NEXT
};

struct create_menu_GUI {
	bool Do = true; 
	Next_Menu next_menu; 
};

enum GUI_TYPE {
	Label,
	Button,
	Input_Text,
	Image,
	Checkbox,
	Slider
}; 

enum Hover_State {
	HOVER,
	CLICK,
	DRAG,
	OUTSIDE
};


struct Hover_Anim {
	SDL_Rect a_Idle;
	SDL_Rect a_Hover;
	SDL_Rect a_Hover_light;
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

	// GUI global
	void Menu_Level_GUI_Manager(); 
	void Clean_Menu_GUI(Active_Menu active_menu);
	void Clean_Level_GUI();
	void Restart_Level_Entities_and_Map(); 

	// Called before quitting
	bool CleanUp();
	
	// GUI Objects
	void Check_Clicked(); 
	void Do_Logic_Hovered(j1Gui_Object*);
	void Do_Logic_Clicked(j1Gui_Object*);


	// Gui creation functions

	j1Gui_Image* Create_Image(SDL_Texture*, iPoint pos, SDL_Rect&, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level, j1Gui_Object* parent = nullptr, bool draggable = false);

	j1Gui_Label* Create_Label(iPoint pos, _TTF_Font*, char*, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level, j1Gui_Object* parent = nullptr, bool draggable = true);

	j1Gui_Button* Create_Button(Hover_Anim& hover_rects, SDL_Texture* tex, iPoint pos, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level, j1Gui_Object* parent = nullptr, bool draggable = true);

	j1Gui_Slider* Create_Slider(SDL_Texture* tex, iPoint pos, SDL_Rect rect, j1Gui_Object* parent, char* ID = nullptr, Menu_Level menu_level = Menu_Level::Level, bool draggable = true);
	

	void Select_Clicked_Object(); 
	j1Gui_Object* Get_Clicked_Object(); 
	p2List<j1Gui_Object*> Get_Clicked_Object_Children(j1Gui_Object*);
	void Move_Clicked_Object(j1Gui_Object*); 
	void Move_Slider(j1Gui_Object*, iPoint dest_pos);
	

	virtual void Blit();
	void Generate_Menu_GUI(); 
	void Generate_Level_GUI();


     const SDL_Texture* GetAtlas() const;


public: 
	bool Main_Created = false;
	bool Sett_Created = false;
	bool Cred_Created = false;
	SDL_Texture* atlas;
	j1Gui_Button* play_button = nullptr;
	j1Gui_Label* play_button_label = nullptr;
	j1Gui_Button* continue_button = nullptr;
	j1Gui_Button* exit_button = nullptr;
	j1Gui_Button* settings_button = nullptr;
	j1Gui_Button* credits_button = nullptr;
	j1Gui_Button* credits_to_main_button = nullptr;
	j1Gui_Button* settings_to_main_button = nullptr;
	j1Gui_Button* settings_to_level_button = nullptr;

	p2SString atlas_file_name;
	p2List<j1Gui_Object*> objects; 

	// getting credits txt 



	// fonts

	_TTF_Font* standard_font = nullptr; 
	_TTF_Font* menu_font_2 = nullptr;
	_TTF_Font* title_font = nullptr;

	// level objects

	j1Gui_Image* UI_coin = nullptr; 
	j1Gui_Label* coin_score = nullptr; 

	j1Gui_Image* UI_lives = nullptr;
	j1Gui_Label* live_count = nullptr;

	j1Gui_Label* time_count = nullptr;

	// main menu objects 
	j1Gui_Label* game_title = nullptr;

	j1Gui_Image* menu_image = nullptr;
	j1Gui_Image* menu_label = nullptr;
	SDL_Texture* menu_image_tex = nullptr;  // put in atlas maybe

	

	j1Gui_Label* continue_button_label = nullptr;


	j1Gui_Label* settings_button_label = nullptr;


	j1Gui_Label* credits_button_label = nullptr;

	
	j1Gui_Label* exit_button_label = nullptr;

	//  credits objects

	j1Gui_Image* credits_image = nullptr;
	SDL_Texture* credits_image_tex = nullptr;

	
	j1Gui_Label* credits_to_main_label = nullptr;

	j1Gui_Label* credits_label = nullptr;

	//  settings objects

	j1Gui_Image* settings_image = nullptr;
	SDL_Texture* settings_image_tex = nullptr;

	
	j1Gui_Label* settings_to_main_label = nullptr;

	
	j1Gui_Label* settings_to_level_label = nullptr;


	j1Gui_Image* settings_mus_bar = nullptr;  
	j1Gui_Slider* settings_mus_slider = nullptr; 


	j1Gui_Image* settings_fx_bar = nullptr;
	j1Gui_Slider* settings_fx_slider = nullptr;

	// hovering and clicking

	uint hover_objects_queue = 0; 
	j1Gui_Object* clicked_object = nullptr; 
	bool move_object = false; 
	bool reset_child_search = false; 

public: 
	create_menu_GUI create_menu_GUI; 
	bool create_level_GUI = false;
	uint creation_level_times = 0; 
	uint cleaned_level_times = 0;

	bool first_start = true; 
	bool Exit_Quits_App = false; 
	bool settings_from_level = false; 
	bool reset_hover_fx = false;
	iPoint mouse_pos;
	iPoint capture_mouse_pos;
	iPoint last_mouse_pos;
	uint frame_count = 0;
	uint cleaned_times = 0;
	bool clicked_in_this_frame = false;

	bool debug_ = false;
public: 
	bool first = true;
	uint coins_collected = 0;                  // Where should this be?

};

#endif // __j1GUI_H__