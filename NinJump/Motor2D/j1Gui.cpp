#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Gui_Label.h"
#include "j1Gui_Image.h"
#include "j1Gui_Object.h"
#include "j1Gui_Button.h"
#include "j1Gui_Slider.h"
#include "j1Main_Menu.h"
#include "j1Scene.h"
#include "j1FadeBlack.h"
#include "j1Entity_Manager.h"
#include "j1Audio.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Collision.h"
#include <string.h>

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	// atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	// textures
	// atlas = App->tex->Load(atlas_file_name.GetString());
	atlas = App->tex->Load("gui/atlas.png");                      // do this in tiled
	menu_image_tex = App->tex->Load("Maps/Textures/bg_menu.png");
	credits_image_tex = App->tex->Load("Maps/Textures/bg_credits.png");
	settings_image_tex = App->tex->Load("Maps/Textures/bg_settings.png");

	// credits file 
	
	


    /*FILE *f = fopen("gui/prueba.txt", "r");

    char line[666]; 
	char* result[50]; 
	uint i = 0; 
	while ((fgets(line, sizeof(line), f) != NULL)) {
		char buffer[sizeof(line)];
		char* val1 = strtok(line, ",");
		char* val2 = strtok(NULL, ",");
		strcpy(buffer, val1);
		result[i] = buffer; 
		LOG("line is ----------------------------------> %s", result[i]);

		i++; 
	}
		
	LOG("file is ----------------------------------> %s", result);

	fclose(f); */
	
	



	                              // the next elements don't have Clean Up
	// fonts
	standard_font = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai.ttf", 36);
	menu_font_2 = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai.ttf", 24);
	title_font = App->font->Load("fonts/shai_fontai/SF_Shai_Fontai_Extended.ttf", 72);
	
	// audio
	App->audio->LoadFx("Sound/Fx/button_hover.wav");
	App->audio->LoadFx("Sound/Fx/button_click.wav");



	return true;
}



bool j1Gui::PreUpdate()
{
	
	Select_Clicked_Object(); 
	Check_Clicked(); 
	
	


	return true;
}

bool j1Gui::Update(float dt) {

	
/*	LOG("                                                          actual DT                   %f", dt); 
	LOG(" 888888888888888888888888888888888888888888888888888888     CREATED LEVEL ----> %i times", creation_level_times);
	LOG(" 888888888888888888888888888888888888888888888888888888     CLEANED LEVEL ----> %i times", cleaned_level_times); */

	Menu_Level_GUI_Manager(); 
	Blit(); 
	
	return true; 
}


void j1Gui::Menu_Level_GUI_Manager() {

	if (create_menu_GUI.Do) {        /// CREATE MENU
		if (first_start) {                                              
			create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT;                
			Generate_Menu_GUI();
			// create_menu_GUI.next_menu = Next_Menu::NONE_NEXT;
			create_menu_GUI.Do = false;
			first_start = false;
		}
		else {
			if (App->main_menu->active_menu == Active_Menu::NONE) { // level to menu

				if (!settings_from_level) {          // when you come from level and go to settings, map must not clean up
					App->map->CleanUp();
				}
					Clean_Level_GUI();   
			}
			else {
				Clean_Menu_GUI(App->main_menu->active_menu);  // menu to menu
			}
			Generate_Menu_GUI();
			create_menu_GUI.Do = false;
		}
	}
	else if (create_level_GUI) {         /// CREATE LEVEL

		if (App->main_menu->active_menu != Active_Menu::NONE) {    // menu to level
			Clean_Menu_GUI(App->main_menu->active_menu);
			
		}
		else {
			Clean_Level_GUI();       // level to level
		}
	
		App->main_menu->active_menu = Active_Menu::NONE; 
		Generate_Level_GUI();
		create_level_GUI = false;
	}
}

void j1Gui::Restart_Level_Entities_and_Map() {

	/*App->scene->Map_Loaded = false;        // restart scene map
    App->entity_manager->restart = true;     // restart entities */

	App->entity_manager->Start();      // start entites 
	
}

void j1Gui::Generate_Menu_GUI() {

	Hover_Anim anim_rects;
	anim_rects.a_Idle = { 1, 39, 70, 88 };
	anim_rects.a_Hover_light = { 525, 33, 129, 155 };
	anim_rects.a_Hover = { 71, 31, 180, 115 };


	if (create_menu_GUI.next_menu == Next_Menu::MAIN_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::MAIN; 

		// images
		menu_image = Create_Image(menu_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 965 }, NULL, Menu_Level::Main_Menu);
		menu_label = Create_Image(atlas, iPoint(450, 30), SDL_Rect{ 2, 149, 573, 293 }, NULL, Menu_Level::Main_Menu, nullptr, true);


		// buttons
		play_button = Create_Button(anim_rects, atlas, iPoint(570, 40), "play_button", Menu_Level::Main_Menu, menu_label);
		continue_button = Create_Button(anim_rects, atlas, iPoint(570, 130), "continue_button", Menu_Level::Main_Menu, menu_label);
		settings_button = Create_Button(anim_rects, atlas, iPoint(570, 220), "settings_button", Menu_Level::Main_Menu, menu_label);

		anim_rects.a_Idle = { 252, 71, 111, 76 };
		anim_rects.a_Hover_light = { 680, 20, 219, 161 };
		anim_rects.a_Hover = { 364, 41, 113, 111 };
		

		credits_button = Create_Button(anim_rects, atlas, iPoint(850, 600), "credits_button", Menu_Level::Main_Menu);
		exit_button = Create_Button(anim_rects, atlas, iPoint(20, 600), "exit_button", Menu_Level::Main_Menu);

		// labels
		game_title = Create_Label(iPoint(100, 400), title_font, "NinJump", NULL, Menu_Level::Main_Menu);

		play_button_label = Create_Label(iPoint(645, 75), standard_font, "PLAY", NULL, Menu_Level::Main_Menu, play_button);
		continue_button_label = Create_Label(iPoint(625, 165), standard_font, "CONTINUE", NULL, Menu_Level::Main_Menu, continue_button);
		settings_button_label = Create_Label(iPoint(628, 255), standard_font, "SETTINGS", NULL, Menu_Level::Main_Menu, settings_button);

		credits_button_label = Create_Label(iPoint(880, 640), menu_font_2, "credits", NULL, Menu_Level::Main_Menu, credits_button);
		exit_button_label = Create_Label(iPoint(60, 640), menu_font_2, "exit", NULL, Menu_Level::Main_Menu, exit_button);
	}


	else if (create_menu_GUI.next_menu == Next_Menu::SETTINGS_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::SETTINGS;

		// images
		settings_image = Create_Image(settings_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 817 }, NULL, Menu_Level::Settings_Menu);

		Create_Image(atlas, iPoint(10, 205), SDL_Rect{ 658, 300, 40, 37 }, NULL, Menu_Level::Settings_Menu);
		Create_Image(atlas, iPoint(450, 205), SDL_Rect{ 657, 259, 41, 38 }, NULL, Menu_Level::Settings_Menu);

		settings_mus_bar = Create_Image(atlas, iPoint(60, 200), SDL_Rect{ 594, 188, 376, 54 }, NULL, Menu_Level::Settings_Menu);
		settings_mus_slider = Create_Slider(atlas, iPoint(67, 208), SDL_Rect{ 593, 256, 45, 38 }, settings_mus_bar, "mus_slider");

		Create_Image(atlas, iPoint(10, 265), SDL_Rect{ 658, 300, 40, 37 }, NULL, Menu_Level::Settings_Menu);
		Create_Image(atlas, iPoint(450, 265), SDL_Rect{ 657, 259, 41, 38 }, NULL, Menu_Level::Settings_Menu);

		settings_fx_bar = Create_Image(atlas, iPoint(60, 260), SDL_Rect{ 594, 188, 376, 54 }, NULL, Menu_Level::Settings_Menu);
		settings_fx_slider = Create_Slider(atlas, iPoint(67, 268), SDL_Rect{ 593, 256, 45, 38 }, settings_fx_bar, "fx_slider"); 


		
		if (!settings_from_level) {
			// buttons
			settings_to_main_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "settings_to_main_button", Menu_Level::Settings_Menu);

			// labels
			settings_to_main_label = Create_Label(iPoint(125, 640), standard_font, "BACK", NULL, Menu_Level::Settings_Menu, settings_to_main_button);
		}
		else {
			// buttons
			settings_to_main_button = Create_Button(anim_rects, atlas, iPoint(500, 600), "settings_to_main_button", Menu_Level::Settings_Menu);

			// labels
			settings_to_main_label = Create_Label(iPoint(565, 640), standard_font, "START", NULL, Menu_Level::Settings_Menu, settings_to_main_button);

			// buttons
			settings_to_level_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "settings_to_level_button", Menu_Level::Settings_Menu);

			// labels
			settings_to_level_label = Create_Label(iPoint(110, 640), standard_font, "RESUME", NULL, Menu_Level::Settings_Menu, settings_to_level_button);
		}


		Create_Label(iPoint(495, 210), standard_font, "music", NULL, Menu_Level::Settings_Menu);
		Create_Label(iPoint(495, 270), standard_font, "fx", NULL, Menu_Level::Settings_Menu);

	}


	else if (create_menu_GUI.next_menu == Next_Menu::CREDITS_NEXT) {

		// assign menu
		App->main_menu->active_menu = Active_Menu::CREDITS;

		// images
		credits_image = Create_Image(credits_image_tex, iPoint(0, 0), SDL_Rect{ 0, 0, 1050, 788 }, NULL, Menu_Level::Credits_Menu);

		// buttons
		credits_to_main_button = Create_Button(anim_rects, atlas, iPoint(50, 600), "credits_to_main_button", Menu_Level::Credits_Menu);

		// labels
		credits_to_main_label = Create_Label(iPoint(125, 640), standard_font, "BACK", NULL, Menu_Level::Credits_Menu, credits_to_main_button);
		
		// credits_label = Create_Label(iPoint(125, 100), standard_font, line, NULL, Menu_Level::Credits_Menu);

	}

	if(create_menu_GUI.Do)
	create_menu_GUI.Do = false;

}


void j1Gui::Generate_Level_GUI() {
	LOG("----------------------------------------------- creating lvl GUI");

	// assingn menu
	App->main_menu->active_menu = Active_Menu::NONE; 
	creation_level_times++; 

	// UI COINS                                            // labels should go after images
	SDL_Rect r = { 0, 0, 32, 32 };
	UI_coin = Create_Image(atlas, iPoint(820, 25), r, NULL, Menu_Level::Level);


	char* ID = "coin_score";
	coin_score = Create_Label(iPoint(860, 30), standard_font, "X0", ID, Menu_Level::Level);


	// UI LIVES 
	ID = "UI_lives";
	r = { 33, 0, 36, 32 };
	UI_lives = Create_Image(atlas, iPoint(940, 25), r, ID, Menu_Level::Level);

	ID = "life_count";
	live_count = Create_Label(iPoint(980, 30), standard_font, "X3", ID, Menu_Level::Level);


	// UI TIME 
	ID = "time_count";
	r = { 50, 0, 36, 32 };
	time_count = Create_Label(iPoint(30, 30), standard_font, "GO! 0", ID, Menu_Level::Level);


	if(create_level_GUI)
	create_level_GUI = false; 

}


void j1Gui::Do_Logic_Hovered(j1Gui_Object* object) {

	if (object->type == GUI_TYPE::Button) {
		if (!reset_hover_fx) {
		//	App->audio->PlayFx(1, 0);
			reset_hover_fx = true;
		}
	}

}

void j1Gui::Check_Clicked() {

	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		
	 	if (item->data->menu_level != Menu_Level::Level) {
			if (item->data->hover_state == Hover_State::HOVER) {
				Do_Logic_Hovered(item->data); 
			}
			else if (item->data->hover_state == Hover_State::CLICK) {
				Do_Logic_Clicked(item->data);
			}
		 }	
	}

}

void j1Gui::Do_Logic_Clicked(j1Gui_Object* object) {        // menu swap TRIGGERS

	// menu buttons 
	if (object->type == GUI_TYPE::Button) {
		LOG("...................................   Clicked button fx   .........");
		App->audio->PlayFx(2, 0); 
	}

	if (object->ID == "play_button") {                                 // go to level

		create_level_GUI = true; // already in scene

		if (settings_from_level) {
			LOG("                                                 ***************        From main to level agan !!!!"); 
			//App->map->Load("Level_1.tmx");
			/*App->scene->Map_Loaded = false; 
			App->entity_manager->Start(); 
		    App->fade->FadeToBlack(App->main_menu, App->scene, 0.5f);*/
			App->scene->MapSwap(0, false);
		}
		 else {
			App->scene->MapSwap(0, first);
		}
		
		

	}

	else if (object->ID == "continue_button") {
		App->LoadGame("save_game.xml");
		create_level_GUI = true;
	}

	else if (object->ID == "exit_button") {       // quit game

		Exit_Quits_App = true;
	}

		else if (object->ID == "settings_button") {                                     // go to settings menu
			if (!create_menu_GUI.Do)
				create_menu_GUI.Do = true;

			create_menu_GUI.next_menu = Next_Menu::SETTINGS_NEXT;
			settings_from_level = false;                                         // flag for settings variant 1 
			App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
		}

		else if (object->ID == "credits_button") {                                       // go to credits menu
			if (!create_menu_GUI.Do)
				create_menu_GUI.Do = true;

			create_menu_GUI.next_menu = Next_Menu::CREDITS_NEXT;
			App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);

		}

		else if (object->ID == "credits_to_main_button") {                               // go to main from credits
			if (!create_menu_GUI.Do)
				create_menu_GUI.Do = true;

			create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT;
			App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);
		}
		else if (object->ID == "settings_to_main_button") {                               

		if (settings_to_main_label->text == "START") {        // in game settings to main (cleans scene)  

			// clean scene

			App->collision->CleanWallDeath();
			App->map->CleanUp(); 
		    App->entity_manager->Disable(); 
			App->scene->Disable();
		}

		if (!create_menu_GUI.Do)
			create_menu_GUI.Do = true;                                         // settings to main

		create_menu_GUI.next_menu = Next_Menu::MAIN_NEXT;
		App->fade->FadeToBlack(App->main_menu, App->main_menu, 1.5f);

		
        }


		else if (object->ID == "settings_to_level_button") {                               // go to main from credits
		create_level_GUI = true;
		App->scene->game_paused = false;                                      // restart dt 

	}

	

}



void j1Gui::Clean_Menu_GUI(Active_Menu acitve_menu){

	cleaned_level_times++; 
	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	bool do_it = false; 
	uint counter = 0; 

	for (item = objects.start; item != NULL; item = item->next)
	{
		if (item->data->menu_level == Menu_Level::Main_Menu && acitve_menu == Active_Menu::MAIN) {
			do_it = true; 
		}
		else if (item->data->menu_level == Menu_Level::Settings_Menu && acitve_menu == Active_Menu::SETTINGS) {
			do_it = true;
		}
		else if (item->data->menu_level == Menu_Level::Credits_Menu && acitve_menu == Active_Menu::CREDITS) {
			do_it = true;
		}
	

		if (do_it) {
			counter++; 
			ret = item->data->CleanUp();
			delete item->data;
			item->data = nullptr;
			objects.del(item);
		}
	}
	
	cleaned_times++; 
	LOG("Free --------------------------------------------------------->   %i gui objs", counter); 
	LOG("Cleaned --------------------------------------------------------->   %i times", cleaned_times);

}

void j1Gui::Clean_Level_GUI() {

	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		if (item->data->menu_level == Menu_Level::Level) {
			ret = item->data->CleanUp();
			delete item->data;
			item->data = nullptr;
			objects.del(item);
		}
		
	}


}


// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

j1Gui_Image* j1Gui::Create_Image(SDL_Texture* tex, iPoint pos, SDL_Rect& atlas_rect, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) {

     j1Gui_Image* ret = new j1Gui_Image(tex, pos, atlas_rect, ID, menu_level, parent, draggable);

	 if(ret != nullptr)
     objects.add(ret); 

	 return ret; 
};


j1Gui_Label* j1Gui::Create_Label(iPoint pos, _TTF_Font* font, char* text, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) {
	
	j1Gui_Label* ret = new j1Gui_Label(pos, font, text, ID, menu_level, parent, draggable);

	if (ret != nullptr)
	objects.add(ret); 

	return ret;
}; 


j1Gui_Button* j1Gui::Create_Button(Hover_Anim& hover_rects, SDL_Texture* tex, iPoint pos, char* ID, Menu_Level menu_level, j1Gui_Object* parent, bool draggable) {

	j1Gui_Button* ret = new j1Gui_Button(hover_rects, tex, pos, ID, menu_level, parent, draggable);

	if (ret != nullptr)
		objects.add(ret);


	return ret; 

}; 


j1Gui_Slider* j1Gui::Create_Slider(SDL_Texture* tex, iPoint pos, SDL_Rect rect, j1Gui_Object* parent, char* ID, Menu_Level menu_level, bool draggable) {


	j1Gui_Slider* ret = new j1Gui_Slider(tex, pos, rect, parent, ID, menu_level, draggable);

	if (ret != nullptr)
		objects.add(ret);


	return ret;


}

void j1Gui::Select_Clicked_Object() {

	int x, y; 

	bool inside = false; 

	p2List<j1Gui_Object*> selected_objects;
	

		App->input->GetMousePosition(x, y); 

		App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
		
		if (frame_count == 0) {
			last_mouse_pos = mouse_pos; 
		}

		p2List_item<j1Gui_Object*>* item;
		item = objects.start;

		SDL_Rect obj_r; 


		/*p2List_item<j1Gui_Object*>* item_c;
		p2List<j1Gui_Object*> childs;
		uint child_count = 0;*/


		for (item = objects.start; item != NULL; item = item->next)
		{
			// reset button fxs

			if (item->data->hover_state != Hover_State::HOVER && reset_hover_fx == true) {
				reset_hover_fx = false;
			}

			obj_r.x = item->data->pos.x;
			obj_r.y = item->data->pos.y;
			obj_r.w = item->data->rect.w;
			obj_r.h = item->data->rect.h;

			// check mouse inside object

			if (x > obj_r.x && x < obj_r.x + obj_r.w && y > obj_r.y && y < obj_r.y + obj_r.h) {

				inside = true; 

				switch (item->data->hover_state) {

				case Hover_State::OUTSIDE:

					item->data->hover_state = Hover_State::HOVER;
					break;

				case Hover_State::HOVER:

					if(item->data->type != GUI_TYPE::Slider)
					move_object = false;

					if (App->input->GetMouseButtonDown(1) == KEY_DOWN && !move_object) {

						item->data->hover_state = Hover_State::CLICK;
					}

					break;


				

				case Hover_State::DRAG:

					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}



					if (item->data != nullptr) {
						clicked_object = item->data;

						if (clicked_object->type == GUI_TYPE::Slider) {
							reset_slider_focus = false; 
						}
					}
					

					if(clicked_object != nullptr && clicked_object->draggable) 
					Move_Clicked_Object(clicked_object);

					break; 
					
					case Hover_State::CLICK:


					clicked_in_this_frame = true; 

					if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						item->data->hover_state = Hover_State::HOVER;
					}

					else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
						item->data->hover_state = Hover_State::DRAG;
					}

					break;
				}
			}
			else {
				item->data->hover_state = Hover_State::OUTSIDE;

				if (clicked_object && clicked_object->type == GUI_TYPE::Slider) {  // keeps slider as clicked 
					clicked_object = item->data;
				}

			}

		}
		 
	
		// move slider if slider bar was clicked

		bool do_it = false; 

		if (clicked_object != nullptr){ 

		   p2List_item<j1Gui_Object*>* item;

			for (item = objects.start; item != NULL; item = item->next) {

				if (item->data->parent == clicked_object && item->data->type == GUI_TYPE::Slider) { 

					if (x < item->data->pos.x || x > item->data->pos.x + item->data->rect.w) {
						do_it = true; 
					}

					if(clicked_in_this_frame && do_it && item->data->parent)
					Move_Slider(item->data, iPoint(x,y));

				}

		    }

        }

		clicked_in_this_frame = false; 


		/*// move slider even if mouse is out of range

		if (!reset_slider_focus) {

			if (clicked_object && clicked_object->type == GUI_TYPE::Slider) {

				LOG("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[``        moving slider off focus ]]]]]]]]]]]]]]]");

				bool do_it = true;

				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					do_it = false;
					reset_slider_focus = true; 
				}

				if (do_it) {
					Move_Clicked_Object(clicked_object);
				
				}

			}

		}*/
		

		// move children

		if (clicked_object) {

			p2List_item<j1Gui_Object*>* item_child;

			for (item_child = objects.start; item_child != NULL; item_child = item_child->next) {

				if (item_child->data && item_child->data->parent) {

					if (item_child->data->parent == clicked_object) {

						if (item_child->data->parent->hover_state == Hover_State::DRAG) {       // move children

							Move_Clicked_Object(item_child->data);
							                                                                 
							item_child->data->moving_with_parent = true;
						}

						else {

							item_child->data->moving_with_parent = false;
						}

					}
					 
					else if (item_child->data->parent->moving_with_parent) {        // move children of children

						Move_Clicked_Object(item_child->data);
					}

				}

			}

		}

	

}





j1Gui_Object* j1Gui::Get_Clicked_Object() {

	return clicked_object;
}


void j1Gui::Move_Clicked_Object(j1Gui_Object* obj) { 


	LOG(" *******************************   moving obj   *******************************"); 

	frame_count++; 

	if (frame_count == 4) {
		last_mouse_pos = mouse_pos; 
	//	frame_count = 0; 
	}

	
	if (last_mouse_pos != mouse_pos) {

		iPoint obj_pos = obj->Get_Pos();
		mouse_pos = App->input->GetActualMousePosition();

		iPoint dist;
		dist.x = mouse_pos.x - obj_pos.x;
		dist.y = mouse_pos.y - obj_pos.y;

		iPoint dist_last_frame_mouse; 
		dist_last_frame_mouse.x = mouse_pos.x - last_mouse_pos.x; 
		dist_last_frame_mouse.y = mouse_pos.y - last_mouse_pos.y;

		iPoint new_pos;
		new_pos.x = (mouse_pos.x) - (dist.x) + dist_last_frame_mouse.x;
		new_pos.y = (mouse_pos.y) - (dist.y) + dist_last_frame_mouse.y;


		if (obj->type == GUI_TYPE::Slider) {

			Move_Slider(obj, new_pos); 
			
		}
		else {
			obj->Set_Pos(new_pos.x, new_pos.y);
		}

		

	}


	if (frame_count == 3) {
		frame_count = 0;
	}

}

void j1Gui::Move_Slider(j1Gui_Object* obj, iPoint new_pos) {

	// bool move_x = true; 

	uint right_offset = 48; 
	uint left_offset = 5;

	uint start_pos;
	uint end_pos;

	if (obj->parent != nullptr) {
		if (new_pos.x >= obj->parent->pos.x + left_offset && new_pos.x <= obj->parent->pos.x + obj->parent->rect.w - right_offset) {
			obj->pos.x = new_pos.x;
		}
	}

	start_pos = obj->parent->pos.x + left_offset; 
	end_pos = obj->parent->pos.x + obj->parent->rect.w - right_offset; 

	uint total_range = end_pos - start_pos;
	uint range_volume_factor = total_range / SDL_MIX_MAXVOLUME; 

	uint volume = (obj->pos.x - start_pos) / range_volume_factor; 

	
	// music volume

	if (obj->ID == "mus_slider") {

		App->audio->Change_Mus_Volume(volume); 

	}

	else if (obj->ID == "fx_slider") {

		App->audio->Change_Fx_Volume(volume); 
	}

	LOG("VOLUME is ------------------------------------------------>>>>> %i", volume);
}


/*
void j1Gui::Delete_Object() {




};*/


void j1Gui::Blit(){

	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) {  // change to f8; 
		debug_ = !debug_; 
	}

	for (item = objects.start; item != NULL; item = item->next)
	{
			 item->data->Blit(); 
	
			 if (debug_) {
				 SDL_Rect r; 
				 r.x = item->data->pos.x; 
				 r.y = item->data->pos.y;
				 r.w = item->data->rect.w;
				 r.h = item->data->rect.h;

				 App->render->DrawQuad(r, 200, 200, 200, 75, true, false); 
		       }


	}

	
};

// Called before quitting
bool j1Gui::CleanUp()
{                                    // as entity manager
	LOG("Freeing GUI");
	bool ret = true;
	p2List_item<j1Gui_Object*>* item;
	item = objects.start;

	for (item = objects.start; item != NULL; item = item->next)
	{
		ret = item->data->CleanUp();
		delete item->data;
		item->data = nullptr;

	}
	objects.clear(); 

	// textures
	App->tex->UnLoad(menu_image_tex);
	App->tex->UnLoad(credits_image_tex);
	App->tex->UnLoad(settings_image_tex);
	App->tex->UnLoad(atlas); 
	// App->font->CleanUp(); 

	// audio
	/*App->audio->UnloadFx(6); 
	App->audio->UnloadFx(7);*/

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}



